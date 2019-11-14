#include "executor/instance/memory.h"
#include "ast/instruction.h"
#include "executor/common.h"
#include "executor/worker.h"
#include "executor/worker/util.h"
#include "support/casting.h"

#include <cstdint>
#include <type_traits>

namespace SSVM {
namespace Executor {

ErrCode Worker::runMemorySizeOp() {
  /// Get Memory Instance
  ErrCode Status = ErrCode::Success;
  Instance::MemoryInstance *MemoryInst = nullptr;
  if ((Status = getMemInstByIdx(0, MemoryInst)) != ErrCode::Success) {
    return Status;
  };

  /// Push SZ = page size to stack.
  return StackMgr.push(MemPool.allocValueEntry(MemoryInst->getDataPageSize()));
}

ErrCode Worker::runMemoryGrowOp() {
  /// Get Memory Instance
  ErrCode Status = ErrCode::Success;
  Instance::MemoryInstance *MemoryInst = nullptr;
  if ((Status = getMemInstByIdx(0, MemoryInst)) != ErrCode::Success) {
    return Status;
  };

  /// Pop N for growing page size.
  std::unique_ptr<ValueEntry> N;
  if ((Status = StackMgr.pop(N)) != ErrCode::Success) {
    return Status;
  }

  /// Grow page and push result.
  unsigned int CurrPageSize = MemoryInst->getDataPageSize();
  if (MemoryInst->growPage(retrieveValue<uint32_t>(*N.get())) !=
      ErrCode::Success) {
    return StackMgr.pushValue(static_cast<uint32_t>(-1));
  }
  MemPool.destroyValueEntry(std::move(N));
  return StackMgr.pushValue(CurrPageSize);
}

} // namespace Executor
} // namespace SSVM
