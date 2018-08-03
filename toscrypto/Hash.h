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
/** @file Hash.h
 * @author Gav Wood <i@gavwood.com>
 * @date 2014
 *
 * The FixedHash fixed-size "hash" container type.
 */

#pragma once

#include <toscore/crypto/FixedHash.h>
#include <toscore/common/vector_ref.h>

namespace dev
{

h256 sha256(bytesConstRef _input) noexcept;

h160 ripemd160(bytesConstRef _input);

}
