/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XSIMD_ERROR_GAMMA_TEST_HPP
#define XSIMD_ERROR_GAMMA_TEST_HPP

#include "xsimd_tester.hpp"
#include "xsimd_test_utils.hpp"

namespace xsimd
{

    template <class T, std::size_t N, std::size_t A>
    struct simd_error_gamma_tester : simd_tester<T, N, A>
    {
        using base_type = simd_tester<T, N, A>;
        using vector_type = typename base_type::vector_type;
        using value_type = typename base_type::value_type;
        using res_type = typename base_type::res_type;

        std::string name;

        res_type input;
        res_type erf_res;
        res_type erfc_res;
        res_type gamma_input;
        res_type tgamma_res;
        res_type lgamma_res;
        res_type gamma_neg_input;
        res_type tgamma_neg_res;
        res_type lgamma_neg_res;

        simd_error_gamma_tester(const std::string& n);
    };

    template <class T, std::size_t N, std::size_t A>
    simd_error_gamma_tester<T, N, A>::simd_error_gamma_tester(const std::string& n)
        : name(n)
    {
        size_t nb_input = N * 10000;
        input.resize(nb_input);
        erf_res.resize(nb_input);
        erfc_res.resize(nb_input);
        gamma_input.resize(nb_input);
        tgamma_res.resize(nb_input);
        lgamma_res.resize(nb_input);
        gamma_neg_input.resize(nb_input);
        tgamma_neg_res.resize(nb_input);
        lgamma_neg_res.resize(nb_input);
        for (size_t i = 0; i < nb_input; ++i)
        {
            input[i] = value_type(-1.5) + i * value_type(3) / nb_input;
            erf_res[i] = std::erf(input[i]);
            erfc_res[i] = std::erfc(input[i]);
            gamma_input[i] = value_type(0.5) + i * value_type(3) / nb_input;
            tgamma_res[i] = std::tgamma(gamma_input[i]);
            lgamma_res[i] = std::lgamma(gamma_input[i]);
            gamma_neg_input[i] = value_type(-3.99) + i * value_type(0.9) / nb_input;
            tgamma_neg_res[i] = std::tgamma(gamma_neg_input[i]);
            lgamma_neg_res[i] = std::lgamma(gamma_neg_input[i]);
        }
    }

    template <class T>
    bool test_simd_error_gamma(std::ostream& out, T& tester)
    {
        using tester_type = T;
        using vector_type = typename tester_type::vector_type;
        using value_type = typename tester_type::value_type;
        using res_type = typename tester_type::res_type;

        vector_type input;
        vector_type vres;
        res_type res(tester.input.size());

        bool success = true;
        bool tmp_success = true;

        std::string val_type = value_type_name<vector_type>();
        std::string shift = std::string(val_type.size(), '-');
        std::string name = tester.name;
        std::string name_shift = std::string(name.size(), '-');
        std::string dash(8, '-');
        std::string space(8, ' ');

        out << dash << name_shift << '-' << shift << dash << std::endl;
        out << space << name << " " << val_type << std::endl;
        out << dash << name_shift << '-' << shift << dash << std::endl << std::endl;

        out << "erf                     : ";
        for (size_t i = 0; i < tester.input.size(); i += tester.size)
        {
            detail::load_vec(input, tester.input, i);
            vres = erf(input);
            detail::store_vec(vres, res, i);
        }
        tmp_success = check_almost_equal(res, tester.erf_res, out);
        success = success && tmp_success;

        out << "erfc                    : ";
        for (size_t i = 0; i < tester.input.size(); i += tester.size)
        {
            detail::load_vec(input, tester.input, i);
            vres = erfc(input);
            detail::store_vec(vres, res, i);
        }
        tmp_success = check_almost_equal(res, tester.erfc_res, out);
        success = success && tmp_success;

        out << "tgamma                  : ";
        for (size_t i = 0; i < tester.gamma_input.size(); i += tester.size)
        {
            detail::load_vec(input, tester.gamma_input, i);
            vres = tgamma(input);
            detail::store_vec(vres, res, i);
        }
        tmp_success = check_almost_equal(res, tester.tgamma_res, out);
        success = success && tmp_success;

        out << "tgamma (negative input) : ";
        for (size_t i = 0; i < tester.gamma_neg_input.size(); i += tester.size)
        {
            detail::load_vec(input, tester.gamma_neg_input, i);
            vres = tgamma(input);
            detail::store_vec(vres, res, i);
        }
        tmp_success = check_almost_equal(res, tester.tgamma_neg_res, out);
        success = success && tmp_success;

#ifndef __APPLE__
        out << "lgamma                  : ";
        for (size_t i = 0; i < tester.gamma_input.size(); i += tester.size)
        {
            detail::load_vec(input, tester.gamma_input, i);
            vres = lgamma(input);
            detail::store_vec(vres, res, i);
        }
        tmp_success = check_almost_equal(res, tester.lgamma_res, out);
        success = success && tmp_success;

        out << "lgamma (negative input) : ";
        for (size_t i = 0; i < tester.gamma_neg_input.size(); i += tester.size)
        {
            detail::load_vec(input, tester.gamma_neg_input, i);
            vres = lgamma(input);
            detail::store_vec(vres, res, i);
        }
        tmp_success = check_almost_equal(res, tester.lgamma_neg_res, out);
        success = success && tmp_success;
#endif
        return success;
    }

}

#endif
