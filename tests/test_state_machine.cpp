// tests/test_state_machine.cpp
#include <gtest/gtest.h>

#include "libftpp.hpp"

enum class TestState
{
    Idle,
    Running,
    Stopped
};

TEST(StateMachineTest, AddStateAndActionExecutesOnUpdate)
{
    StateMachine<TestState> sm;
    sm.addState(TestState::Idle);

    bool actionCalled = false;
    sm.addAction(TestState::Idle, [&]() { actionCalled = true; });

    EXPECT_NO_THROW(sm.update());
    EXPECT_TRUE(actionCalled);
}

TEST(StateMachineTest, AddTransitionAndTriggerUpdatesStateAndExecutesLambda)
{
    StateMachine<TestState> sm;
    sm.addState(TestState::Idle);
    sm.addState(TestState::Running);

    bool transitionCalled = false;
    sm.addTransition(TestState::Idle, TestState::Running, [&]() { transitionCalled = true; });

    // transitionTo doit réussir et exécuter la lambda
    EXPECT_NO_THROW(sm.transitionTo(TestState::Running));
    EXPECT_TRUE(transitionCalled);

    // Après la transition, update() doit exécuter l'action de Running si définie :
    bool runningAction = false;
    sm.addAction(TestState::Running, [&]() { runningAction = true; });
    EXPECT_NO_THROW(sm.update());
    EXPECT_TRUE(runningAction);
}

TEST(StateMachineTest, TransitionWithoutConfiguredLambdaThrows)
{
    StateMachine<TestState> sm;
    sm.addState(TestState::Idle);
    sm.addState(TestState::Running);

    // Pas de transition définie entre Idle et Running : doit lancer
    EXPECT_THROW(sm.transitionTo(TestState::Running), std::invalid_argument);
}

TEST(StateMachineTest, UpdateWithoutActionThrows)
{
    StateMachine<TestState> sm;
    sm.addState(TestState::Idle);

    // Aucune action définie pour l'état courant (Idle) → update doit jeter
    EXPECT_THROW(sm.update(), std::invalid_argument);
}

TEST(StateMachineTest, MultipleTransitionsAndActionsSequence)
{
    StateMachine<TestState> sm;
    sm.addState(TestState::Idle);
    sm.addState(TestState::Running);
    sm.addState(TestState::Stopped);

    bool idleCalled    = false;
    bool runningCalled = false;
    bool stoppedCalled = false;

    sm.addAction(TestState::Idle, [&]() { idleCalled = true; });
    sm.addAction(TestState::Running, [&]() { runningCalled = true; });
    sm.addAction(TestState::Stopped, [&]() { stoppedCalled = true; });

    sm.addTransition(TestState::Idle, TestState::Running, [&]() {});
    sm.addTransition(TestState::Running, TestState::Stopped, [&]() {});

    // initial state = Idle -> update calls Idle action
    EXPECT_NO_THROW(sm.update());
    EXPECT_TRUE(idleCalled);

    // Idle -> Running
    EXPECT_NO_THROW(sm.transitionTo(TestState::Running));
    EXPECT_NO_THROW(sm.update());
    EXPECT_TRUE(runningCalled);

    // Running -> Stopped
    EXPECT_NO_THROW(sm.transitionTo(TestState::Stopped));
    EXPECT_NO_THROW(sm.update());
    EXPECT_TRUE(stoppedCalled);
}