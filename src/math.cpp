/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 17:52:59 by lrosa-do          #+#    #+#             */
/*   Updated: 2023/02/18 07:33:42 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "math.hpp"


const Mat3 Mat3::ZERO(0,0,0,0,0,0,0,0,0);
const Mat3 Mat3::IDENTITY(1,0,0,0,1,0,0,0,1);




Mat3 createMatrix3(const  Mat4 &m)
{
    Mat3 result;
    result.m[0][0] = m.m[0][0];
    result.m[0][1] = m.m[0][1];
    result.m[0][2] = m.m[0][2];
    result.m[1][0] = m.m[1][0];
    result.m[1][1] = m.m[1][1];
    result.m[1][2] = m.m[1][2];
    result.m[2][0] = m.m[2][0];
    result.m[2][1] = m.m[2][1];
    result.m[2][2] = m.m[2][2];
    return result;
}

Mat4 createMatrix4(const  Mat3 &m)
{
    Mat4 result;
    result.m[0][0] = m.m[0][0];
    result.m[0][1] = m.m[0][1];
    result.m[0][2] = m.m[0][2];
    result.m[0][3] = 0.0f;
    result.m[1][0] = m.m[1][0];
    result.m[1][1] = m.m[1][1];
    result.m[1][2] = m.m[1][2];
    result.m[1][3] = 0.0f;
    result.m[2][0] = m.m[2][0];
    result.m[2][1] = m.m[2][1];
    result.m[2][2] = m.m[2][2];
    result.m[2][3] = 0.0f;
    result.m[3][0] = 0.0f;
    result.m[3][1] = 0.0f;
    result.m[3][2] = 0.0f;
    result.m[3][3] = 1.0f;
    return result;
}
