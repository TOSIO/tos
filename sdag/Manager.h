#pragma once
#include <stdio.h>
#include <toscore/common/Common.h>
#include <toscore/common/Address.h>
#include <boost/optional.hpp>
#include <toscore/utils/RLP.h>
#include <string>
#include "block.h"

namespace dev
{
namespace sdag
{
  
class Manager
{
  public:
  BlockRef creatTransaction();
  addBlock(BlockRef tmpblock);
};

} // namespace sdag
} 

