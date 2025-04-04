/*******************************************************************************
 * fitness_type.hpp: Interface for fitness_t structure.
 *
 * (c) Copyright 2022-2023, Carlos Eduardo de Andrade.
 * All Rights Reserved.
 *
 * Created on : May 18, 2021 by ceandrade.
 * Last update: Sep 20, 2023 by ceandrade.
 *
 * This code is released under BRKGA-MP-IPR License:
 * https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/LICENSE.md
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#ifndef BRKGA_MP_IPR_FITNESS_TYPE_HPP_
#define BRKGA_MP_IPR_FITNESS_TYPE_HPP_

#include <iostream>
#include <limits>
#include <tuple>
#include <utility>

namespace BRKGA {

/**
 * \brief Fitness type representation.
 *
 * In general, fitness_t is a single number for single-objective problems.
 * For instance:
 *
 * \code{.cpp}
 * using fitness_t = double;
 * \endcode
 *
 * For multi-objective problems (with dominance/lexicographical sorting), we
 * need to use multiple values. We can either use a class, structure, or
 * `std::tuple`. Using a custom class or structure, we must provide copy
 * assignment (`operator=`), comparison operators (`operator<`, `operator>`,
 * and `operator==`), and the streaming operator
 * `std::ostream& operator<<(std::ostream& os, const CustomFitness& fitness)`
 * where `CustomFitness` is your fitness structure. We have all these perks
 * using `std:tuple`. For example, assume we have three minimization objective
 * functions. Then we may have:
 *
 * \code{.cpp}
 * using fitness_t = std::tuple<double, double, double>;
 * \endcode
 *
 * \note We do recommend use `std::tuple`.
 *
 * Internally, BRKGA-MP-IPR doesn't use `operator==` directly. BRKGA implements
 * the custom function `close_enough()`. For fundamental numerical types, it
 * compares the absolute difference with a given #EQUALITY_THRESHOLD, i.e., two
 * numbers \f$a\f$ and \f$b\f$ equal if \f$|a - b| < EQUALITY\_THRESHOLD\f$.
 * For all other types (except `std::tuple`), we use `operator==`.
 * For `std::tuple`, we have a specialized function `close_enough()` that
 * iterates over each element of the tuples calling the base `close_enough()`.
 *
 * \warning
 *      If you are using custom class other than fundamental types or
 *      tuples with fundamental types, you must also provide two const template
 *      expressions #FITNESS_T_MIN and #FITNESS_T_MAX.
 */
using fitness_t = double;

namespace { // Hide from external usage.
/**
 * \brief Minimum value template of the #fitness_t (generic).
 *
 * Defines a template for the minimum value for the fitness type. We could use
 * `std::numeric_limits<>::min()`. However, it is not recommended to overload
 * such templates for no-fundamental types such as `std::tuple` or custom
 * classes. Therefore, we provide our own templates.
 *
 * \warning
 *      We have provided a template for single types and a specialized
 *      template for tuples with fundamental types. For any other custom type,
 *      the user must provide his/her own template.
 */
template <class T>
constexpr T FITNESS_T_MIN_TEMPLATE = std::numeric_limits<T>::min();

/**
 * \brief Minimum value of the #fitness_t (tuple specialization).
 *
 * NOTE: this only works if the tuple contains fundamental types.
 */
template <typename... T>
constexpr std::tuple<T...> FITNESS_T_MIN_TEMPLATE<std::tuple<T...>> =
    std::make_tuple(std::numeric_limits<T>::min()...);

/**
 * \brief Maximum value template of the #fitness_t (generic).
 *
 * Defines a template for the maximum value for the fitness type. We could use
 * `std::numeric_limits<>::min()`. However, it is not recommended to overload
 * such templates for no-fundamental types such as `std::tuple` or custom
 * classes. Therefore, we provide our own templates.
 *
 * \note
 *      We have provided a template for single types and a specialized
 *      template for tuples with fundamental types. For any other custom type,
 *      the user must provide his/her own template.
 */
template <class T>
constexpr T FITNESS_T_MAX_TEMPLATE = std::numeric_limits<T>::max();

/**
 * \brief Maximum value of the #fitness_t (tuple specialization).
 *
 * \note This only works if the tuple contains fundamental types.
 */
template <typename... T>
constexpr std::tuple<T...> FITNESS_T_MAX_TEMPLATE<std::tuple<T...>> =
    std::make_tuple(std::numeric_limits<T>::max()...);
} // end hidden namespace

/// The actual minimal value to `fitness_t`.
static constexpr fitness_t FITNESS_T_MIN = FITNESS_T_MIN_TEMPLATE<fitness_t>;

/// The actual Maximum value to `fitness_t`.
static constexpr fitness_t FITNESS_T_MAX = FITNESS_T_MAX_TEMPLATE<fitness_t>;

//----------------------------------------------------------------------------//

/**
 * This constant is used to compare floating-point numbers to equality.
 * Therefore, we consider two numbers \f$a\f$ and \f$b\f$ equal
 * if \f$|a - b| < EQUALITY\_THRESHOLD\f$.
 *
 * Currently, this constant is only used during IPR procedures to compare
 * fitness with fundamental types (int, flot, char, etc), either single type or
 * embedded in a tuple. If your `fitness_t` has a custom type, this is not
 * applied, as explained in #fitness_t.
 */
constexpr double EQUALITY_THRESHOLD = 1e-6;

} // end namespace BRKGA_MP_IPR

//----------------------------------------------------------------------------//

/** \name I/O helpers for tuples.
 * Define some helpers functions to print tuples.
 */
//@{

// For GCC, we must inject these direct into BRKGA namespace.
#ifndef __clang__
namespace BRKGA {
#endif

namespace { // Hide from external usage.
/** \brief the "recursive" helper function to print tuples.
* \tparam Is the number of types in the tuple template.
 * \tparam Ts the tuple types.
 * \param os an output stream object.
 * \param tp the tuple.
 */
template <std::size_t... Is, typename... Ts>
void print_tuple(std::ostream& os, const std::tuple<Ts...>& tp,
                 std::index_sequence<Is...>) {
    ((os << (Is == 0 ? "" : ", "), os << std::get<Is>(tp)), ...);
}
} // end hidden namespace

/** \brief Output streaming operator for tuples.
 * \tparam Ts the tuple types.
 * \param os an output stream object.
 * \param tp the tuple.
 * \return a reference to the output stream object.
 */
template <typename... Ts>
std::ostream& operator<<(std::ostream& os, const std::tuple<Ts...>& tp) {
    os << "(";
    print_tuple(os, tp, std::index_sequence_for<Ts...>());
    os << ")";
    return os;
}
#ifndef __clang__
} // end of namespace BRKGA
#endif
//@}

#endif // BRKGA_MP_IPR_FITNESS_TYPE_HPP_
