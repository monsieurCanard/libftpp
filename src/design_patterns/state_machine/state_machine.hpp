#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

#include <functional>
#include <map>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

template <typename TState>
class StateMachine
{
private:
    TState                                                     _currentS;
    std::unordered_set<TState>                                 _states;
    std::unordered_map<TState, std::function<void()>>          _actions;
    std::map<std::pair<TState, TState>, std::function<void()>> _transitions;

public:
    void addState(const TState& state)
    {
        if (_states.empty())
        {
            _currentS = state;
        }
        _states.insert(state);
    }

    void addTransition(const TState&                startState,
                       const TState&                finalState,
                       const std::function<void()>& lambda)
    {
        if (_states.find(startState) == _states.end() || _states.find(startState) == _states.end())
        {
            throw std::invalid_argument("Invalid State");
        }
        _transitions[std::make_pair(startState, finalState)] = lambda;
    }

    void addAction(const TState& state, const std::function<void()>& lambda)
    {
        _actions[state] = lambda;
    }

    void transitionTo(const TState& state)
    {
        auto it = _transitions.find(std::make_pair(_currentS, state));

        (it == _transitions.end()) ? throw std::invalid_argument("No transition found")
                                   : it->second();
        _currentS = state;
    }

    void update()
    {
        auto it = _actions.find(_currentS);
        (it == _actions.end()) ? throw std::invalid_argument("No action found") : it->second();
    }
};
#endif