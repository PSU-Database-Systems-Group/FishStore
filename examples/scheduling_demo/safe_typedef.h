// Same typedef
//
// Created by Max Norfolk on 11/2/23.


#pragma once

template<typename T, typename TAG>
class Value {
public:
    explicit Value(T val)
            : value_(val) {}

    explicit operator T() const noexcept {
        return value_;
    }

private:
    T value_;
};