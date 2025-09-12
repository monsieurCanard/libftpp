// LoggerTest.cpp
#include <gtest/gtest.h>

#include <cstdio> // pour remove()
#include <fstream>
#include <string>

#include "libftpp.hpp"

class LoggerTest : public ::testing::Test
{
protected:
    std::string logFileName;

    void SetUp() override
    {
        // Nom du fichier temporaire
        logFileName = "test_log";
        Logger::instance().setOutputFile(logFileName);
        Logger::instance().setLogLevel(LogLevel::DEBUG);
    }

    void TearDown() override
    {
        // Supprime le fichier après test
        std::string fullPath = logFileName + ".log";
        std::remove(fullPath.c_str());
    }

    bool fileExists(const std::string& path)
    {
        std::ifstream file(path);
        return file.is_open();
    }

    std::string readFile(const std::string& path)
    {
        std::ifstream file(path);
        std::string   content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        return content;
    }
};

// Test si le fichier est créé
TEST_F(LoggerTest, FileCreation)
{
    Logger::instance().log(LogLevel::INFO, "Test message");
    EXPECT_TRUE(fileExists(Logger::instance().getOutputPathFile()));
}

// Test si le message est écrit dans le fichier
TEST_F(LoggerTest, FileContent)
{
    Logger::instance().log(LogLevel::INFO, "HelloFile");
    std::string content = readFile(Logger::instance().getOutputPathFile());
    EXPECT_NE(content.find("HelloFile"), std::string::npos);
    EXPECT_NE(content.find("INFO"), std::string::npos);
}

// Test du filtrage par niveau
TEST_F(LoggerTest, LevelFiltering)
{
    Logger::instance().setLogLevel(LogLevel::WARNING);
    Logger::instance().log(LogLevel::INFO, "ShouldNotAppear");
    Logger::instance().log(LogLevel::ERROR, "ShouldAppear");

    std::string content = readFile(Logger::instance().getOutputPathFile());
    EXPECT_EQ(content.find("ShouldNotAppear"), std::string::npos);
    EXPECT_NE(content.find("ShouldAppear"), std::string::npos);
}

// Test console logging (basique)
TEST_F(LoggerTest, ConsoleLogging)
{
    Logger::instance().logConsole(LogLevel::INFO, "HelloConsole");
    SUCCEED(); // Vérifie que l'appel ne crash pas
}
