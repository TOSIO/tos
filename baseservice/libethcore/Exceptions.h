/*
	This file is part of tos.

	tos is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	tos is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with tos.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file Exceptions.h
 * @author Christian <c@ethdev.com>
 * @date 2016
 */

#pragma once

#include <toscore/common/Exceptions.h>

namespace dev
{
namespace crypto
{

/// Rare malfunction of cryptographic functions.
DEV_SIMPLE_EXCEPTION(baseserviceException);

}
}
