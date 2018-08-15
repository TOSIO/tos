#pragma once
#include <toscore/storage/db.h>
#include <Block.h>

namespace dev
{
namespace sdag
{
class BlockStorage
{
  public:
    BlockStorage(const std::string& path);
    
    void write(const Block &block);

    std::string read(db::Slice slice);

  private:
    std::unique_ptr<db::DatabaseFace> m_blocksDB;
};
} // namespace sdag
} // namespace dev
