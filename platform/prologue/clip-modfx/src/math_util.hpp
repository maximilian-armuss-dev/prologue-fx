template<typename T>
inline T clamp(T min, T value, T max) {
    return (value < min) ? min : (value > max) ? max : value;
}