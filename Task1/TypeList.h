#pragma once

/******************************************************************************
* ПЕРЕВОД ЧИСЛА В ТИП
******************************************************************************/
template <BigUint::NumericT Value>
struct NumToType
{
    static constexpr BigUint::NumericT value = Value;
};

template <BigUint::NumericT Value>
inline constexpr BigUint::NumericT NumToType_v = NumToType<Value>::value;

/******************************************************************************
* СПИСОК ТИПОВ
******************************************************************************/
template <typename T, typename U>
struct List
{
    using Head = T;
    using Tail = U;
};

// Нулевой список
struct Nil {};

// Длина списка
template <class TList> struct Length;

template <>
struct Length<Nil>
{
    static constexpr size_t value = 0;
};

template <typename T, typename U>
struct Length<List<T, U>>
{
    static constexpr size_t value = 1U + Length<U>::value;
};

template <class TList>
inline constexpr size_t Length_v = Length<TList>::value;

// Удаление элемента
template <class List, typename T> struct Remove;

template <typename T>
struct Remove<Nil, T>
{
    using type = Nil;
};

template <typename T, typename U>
struct Remove<List<T, U>, T>
{
    using type = U;
};

template <typename T, typename U, typename V>
struct Remove<List<T, U>, V>
{
    using type = List<T, typename Remove<U, V>::type>;
};

template <class List, typename T>
using Remove_t = Remove<List, T>::type;

// Доступ по индексу
template <class TList, size_t Index> struct At;

template <typename T, typename U>
struct At<List<T, U>, 0>
{
    using type = T;
};

template <typename T, typename U, size_t Index>
struct At<List<T, U>, Index>
{
    using type = typename At<U, Index - 1>::type;
};

template <class List, size_t Index>
using At_t = At<List, Index>::type;

/******************************************************************************
* ПОСТРОИТЕЛЬ СПИСКА ИЗ ПАРАМЕТРОВ
******************************************************************************/
template <typename T, typename... Ts>
struct ListFactory
{
    using type = List<T, ListFactory<Ts...>::value>;
};

template <typename T>
struct ListFactory<T>
{
    using type = List<T, Nil>;
};

template <typename T, typename... Ts>
using MakeTypeList = ListFactory<T, Ts>::type;

/******************************************************************************
* РЕАЛИЗАЦИЯ ПОДСЧЕТА ЛИТЕРАЛА
******************************************************************************/
#if false

template <BigUint::NumericT N, BigUint::NumericT... Ns>
using MakeNumList = ListFactory<NumToType<N>, NumToType<Ns>...>::type;

typename MakeTypeList<char, short, int> abc;
typename MakeTypeList<char> abc12;
typename MakeNumList<1, 2, 3> abc31;

inline static constexpr NumericT nextNumber(NumericT number, NumericT character) noexcept
{
    return number + character - '0';
}

inline static constexpr NumericT cutNumber(NumericT number, NumericT character) noexcept
{
    return getHiBits(number + character - '0');
}

template <size_t Counter, NumericT N, NumericT R, NumericT X, NumericT... Xs> struct LiteralImpl;

template <NumericT... Chars>
struct Literal
{
    static const NumericT value[] = LiteralImpl<sizeof...(Chars), 0, 0, Chars...>::value;
};

// Терминальный случай
template <NumericT N, NumericT R, NumericT X, NumericT... Xs>
struct LiteralImpl<0, N, R, X, Xs>
{
    static const NumericT value[] = { N + X - '0', Xs... };
};

// Остаток == 0
template <size_t Counter, NumericT N, NumericT X, NumericT... Xs>
struct LiteralImpl<Counter, N, 0, X, Xs>
{
    static const NumericT value[] = LiteralImpl<Counter - 1, N + X - '0', getHiBits(N + X - '0'), Xs...>::value;
};

// Общий случай
template <size_t Counter, NumericT N, NumericT R, NumericT X, NumericT... Xs>
struct LiteralImpl<Counter, N, R, X, Xs>
{
    static const NumericT value[] = LiteralImpl<Counter - 1, N + X - '0', getHiBits(N + X - '0'), Xs..., N>::value;
};

#endif