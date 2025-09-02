// tests/test_state_machine.cpp
#include <gtest/gtest.h>

#include "libftpp.hpp"

enum class TestState
{
    Idle,
    Running,
    Stopped
};

// Si la StateMachine initialise l'état courant au premier état ajouté,
// ces tests supposent ce comportement (c'est la manière la plus logique).
// Les tests vérifient les effets observables (lambdas appelées) plutôt
// qu'un accès direct à l'état interne.

TEST(StateMachineTest, AddStateAndActionExecutesOnUpdate)
{
    StateMachine<TestState> sm;
    sm.addState(TestState::Idle);

    bool actionCalled = false;
    sm.addAction(TestState::Idle, [&]() { actionCalled = true; });

    // Si la machine initialise l'état courant au premier état ajouté,
    // update() doit appeler l'action de Idle.
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
    EXPECT_THROW(sm.transitionTo(TestState::Running), std::runtime_error);
}

TEST(StateMachineTest, UpdateWithoutActionThrows)
{
    StateMachine<TestState> sm;
    sm.addState(TestState::Idle);

    // Aucune action définie pour l'état courant (Idle) → update doit jeter
    EXPECT_THROW(sm.update(), std::runtime_error);
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

TEST(StateMachineTest, TransitionToUnknownStateThrows)
{
    StateMachine<TestState> sm;
    sm.addState(TestState::Idle);

    // On tente de transitionTo vers un état qui n'a pas été ajouté → comportement
    // attendu : lancer une exception (implémentation doit vérifier validité)
    EXPECT_THROW(sm.transitionTo(TestState::Running), std::runtime_error);
}
