// tests/test_thread_safe_iostream.cpp
#include <gtest/gtest.h>

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "thread_safe_iostream.hpp"

// ---------- Helpers RAII pour capturer cout / fournir cin ----------
struct CoutCapture
{
    std::ostringstream oss;
    std::streambuf*    old = nullptr;
    CoutCapture()
    {
        old = std::cout.rdbuf(oss.rdbuf());
    }
    ~CoutCapture()
    {
        std::cout.rdbuf(old);
    }
    std::string str() const
    {
        return oss.str();
    }
};

struct CinFeeder
{
    std::istringstream iss;
    std::streambuf*    old = nullptr;
    explicit CinFeeder(const std::string& data) : iss(data)
    {
        old = std::cin.rdbuf(iss.rdbuf());
    }
    ~CinFeeder()
    {
        std::cin.rdbuf(old);
    }
};

// ---------- Utilitaires pour tests ----------
static std::vector<std::string> split_lines(const std::string& s)
{
    std::vector<std::string> lines;
    size_t                   start = 0;
    while (true)
    {
        size_t pos = s.find('\n', start);
        if (pos == std::string::npos)
            break;
        lines.push_back(s.substr(start, pos - start));
        start = pos + 1;
    }
    // si il reste des données sans trailing newline, on l'ajoute aussi
    if (start < s.size())
        lines.push_back(s.substr(start));
    return lines;
}

static size_t count_substr(const std::string& s, const std::string& needle)
{
    size_t pos = 0, count = 0;
    while ((pos = s.find(needle, pos)) != std::string::npos)
    {
        ++count;
        pos += needle.size();
    }
    return count;
}

static bool starts_with(const std::string& s, const std::string& pref)
{
    return s.rfind(pref, 0) == 0;
}

// ---------- Tests ----------

TEST(ThreadSafeIOStream_Basics, PrefixAtLineStartAndSingleOccurrence)
{
    CoutCapture cap;
    threadSafeCout.setPrefix("[T] ");
    threadSafeCout << "Hello" << std::endl;

    const std::string out   = cap.str();
    auto              lines = split_lines(out);
    ASSERT_FALSE(lines.empty());
    const std::string& line = lines.front();

    // Le préfixe doit être au début de la ligne
    ASSERT_TRUE(starts_with(line, "[T] "));
    // Le préfixe ne doit apparaître qu'une seule fois par ligne
    ASSERT_EQ(count_substr(line, "[T] "), 1u);
    // Le contenu attendu doit être présent dans l'ordre
    ASSERT_NE(line.find("Hello"), std::string::npos);
}

TEST(ThreadSafeIOStream_Basics, ChainedInsertionsProduceSinglePrefixedLine)
{
    CoutCapture cap;
    threadSafeCout.setPrefix("[X] ");
    threadSafeCout << "A" << ' ' << 42 << ' ' << 3.14 << std::endl;

    const std::string out   = cap.str();
    auto              lines = split_lines(out);
    ASSERT_FALSE(lines.empty());
    const std::string& line = lines.front();

    ASSERT_TRUE(starts_with(line, "[X] "));
    ASSERT_EQ(count_substr(line, "[X] "), 1u);

    // vérifie la présence et l'ordre des tokens "A", "42", "3.14"
    size_t pA  = line.find("A");
    size_t p42 = line.find("42", pA == std::string::npos ? 0 : pA);
    size_t pPi = line.find("3.14", p42 == std::string::npos ? 0 : p42);
    ASSERT_NE(pA, std::string::npos);
    ASSERT_NE(p42, std::string::npos);
    ASSERT_NE(pPi, std::string::npos);
    ASSERT_TRUE(pA < p42 && p42 < pPi);
}

TEST(ThreadSafeIOStream_Manipulators, SupportsFlushWithoutNewline)
{
    CoutCapture cap;
    threadSafeCout.setPrefix("[F] ");
    threadSafeCout << "Buffered" << std::flush;

    const std::string out = cap.str();
    // contient le préfixe et la donnée, pas nécessairement '\n'
    ASSERT_NE(out.find("[F] Buffered"), std::string::npos);
}

TEST(ThreadSafeIOStream_Input, ReadsValidInt)
{
    CinFeeder   feed("123\n");
    CoutCapture cap;

    int x = 0;
    threadSafeCout >> x;

    EXPECT_EQ(x, 123);
    // pas de message d'erreur attendu
    EXPECT_EQ(cap.str().find("Invalid input"), std::string::npos);
}

TEST(ThreadSafeIOStream_Input, RecoverFromInvalidThenValid)
{
    CinFeeder   feed("abc\n42\n");
    CoutCapture cap;

    int x = 0;
    threadSafeCout >> x;

    EXPECT_EQ(x, 42);
    // Doit avoir émis au moins un message d'erreur pour la saisie invalide
    EXPECT_NE(cap.str().find("Invalid input"), std::string::npos);
}

TEST(ThreadSafeIOStream_Prompt, WritesQuestionAndReadsAnswer)
{
    CinFeeder   feed("Bob\n");
    CoutCapture cap;

    std::string name;
    threadSafeCout.setPrefix("[P] ");
    threadSafeCout.prompt("Name? ", name);

    EXPECT_EQ(name, "Bob");
    // on attend que la question soit affichée (avec ou sans préfixe)
    EXPECT_NE(cap.str().find("Name? "), std::string::npos);
}

TEST(ThreadSafeIOStream_ThreadLocal, EachThreadHasOwnPrefix)
{
    CoutCapture cap;

    const int perThreadLines = 40;
    auto      worker         = [&](const std::string& prefix, const std::string& tag)
    {
        threadSafeCout.setPrefix(prefix);
        for (int i = 0; i < perThreadLines; ++i)
        {
            threadSafeCout << tag << " " << i << std::endl;
        }
    };

    std::thread t1(worker, "[A] ", "apple");
    std::thread t2(worker, "[B] ", "banana");
    t1.join();
    t2.join();

    const std::string out = cap.str();

    // vérifie qu'on a au moins perThreadLines occurrences de chaque préfixe
    EXPECT_GE(count_substr(out, "[A] "), static_cast<size_t>(perThreadLines));
    EXPECT_GE(count_substr(out, "[B] "), static_cast<size_t>(perThreadLines));
}

TEST(ThreadSafeIOStream_Reentrancy, LinesAreWholeAndNotFragmented)
{
    CoutCapture cap;

    const int N      = 80;
    auto      worker = [&](const std::string& prefix, const std::string& word)
    {
        threadSafeCout.setPrefix(prefix);
        for (int i = 0; i < N; ++i)
        {
            threadSafeCout << word << " " << i << std::endl;
        }
    };

    std::thread t1(worker, "[W1] ", "alpha");
    std::thread t2(worker, "[W2] ", "beta");
    t1.join();
    t2.join();

    const std::string out = cap.str();
    // vérifie la présence de quelques lignes entières représentatives
    EXPECT_NE(out.find("[W1] alpha 0\n"), std::string::npos);
    EXPECT_NE(out.find("[W2] beta 0\n"), std::string::npos);
}

// // ---------- main ----------
// int main(int argc, char** argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
