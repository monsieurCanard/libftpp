// #include <gtest/gtest.h>

// #include <stdexcept>

// #include "libftpp.hpp"

// TEST(MessageTest, TypeIsCorrect)
// {
//     Message msg(42);
//     EXPECT_EQ(msg.type(), 42);
// }

// TEST(MessageTest, WriteAndReadInt)
// {
//     Message msg(1);
//     int     valueOut = 0;
//     int     valueIn  = 12345;

//     msg << valueIn;
//     msg.reset();
//     msg >> valueOut;

//     EXPECT_EQ(valueOut, valueIn);
// }

// TEST(MessageTest, WriteAndReadFloat)
// {
//     Message msg(2);
//     float   valueOut = 0.0f;
//     float   valueIn  = 3.14f;

//     msg << valueIn;
//     msg.reset();
//     msg >> valueOut;

//     EXPECT_FLOAT_EQ(valueOut, valueIn);
// }

// TEST(MessageTest, WriteAndReadMultipleTypes)
// {
//     Message msg(3);
//     int     iIn = 42, iOut = 0;
//     double  dIn = 6.28, dOut = 0.0;

//     msg << iIn << dIn;
//     msg.reset();
//     msg >> iOut >> dOut;

//     EXPECT_EQ(iOut, iIn);
//     EXPECT_DOUBLE_EQ(dOut, dIn);
// }

// TEST(MessageTest, OutOfBoundsThrows)
// {
//     Message msg(4);
//     int     big = 999;
//     // Simule un mauvais _cursor en forçant une lecture hors buffer
//     EXPECT_THROW(
//         {
//             msg.reset();
//             msg >> big;
//         },
//         std::runtime_error);
// }

// TEST(MessageTest, ResetWorks)
// {
//     Message msg(5);
//     int     valIn = 2025, valOut = 0;

//     msg << valIn;
//     msg.reset();
//     msg >> valOut;

//     EXPECT_EQ(valOut, valIn);

//     msg.reset(); // On repart au début
//     valOut = 0;
//     msg >> valOut;
//     EXPECT_EQ(valOut, valIn); // relu encore une fois
// }
