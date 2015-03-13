// Copyright (c) 2015 FAT-GYFT, MIT License

#ifndef SRC_STATES_H_
#define SRC_STATES_H_

struct State {
 public:
    // Note: the missing states are reducing states (see automaton.pdf)
    enum Id {
        E0, E2, E3, E5, E6, E7, E8, E9, E12, E13, E14, E17, E18, E19,
        E20, E22, E23, E24, E25, E26, E28, E29, E31, E32, E33, E34, E36,
        E38, E41, E42, E43, E44, E45
    };

    State(State::Id value) : m_value(value) { } // NOLINT

    operator State::Id() const { return m_value; }

    State::Id m_value;
};

#endif  // SRC_STATES_H_
