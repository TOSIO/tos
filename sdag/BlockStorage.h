#pragma once
#include <toscore/storage/db.h>
#include <Block.h>

namespace dev
{
  db::Slice toSlice(h256 const &_h, unsigned _sub = 0);
  db::Slice toSlice(uint64_t _n, unsigned _sub = 0);

namespace sdag
{
class BlockStorage
{
public:
  BlockStorage(const std::string &path);

  void write(Block &block);
  void write(std::vector<Block *> &blocks);

  std::string read(db::Slice slice);

protected:
  

private:
  std::unique_ptr<db::DatabaseFace> m_blocksDB;
};
} // namespace sdag
} // namespace dev
