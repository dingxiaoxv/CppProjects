#ifndef _CNR_H_201404061148_
#define _CNR_H_201404061148_

#include <cstdint>

#define MAX_PATH 256

#define INVALID_SOCKET (~0)
#define SOCKET_ERROR (~0)

#define INCRC(x) (__sync_fetch_and_add(x, 1))
#define DECRC(x) (__sync_fetch_and_sub(x, 1))

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

template <bool>
struct CompileAssert {};

#define COMPILE_ASSERT(expr, msg) typedef CompileAssert<(bool(expr))> msg[bool(expr) ? 1 : -1]

#define CNR_LONGLONG(x) x##LL
#define CNR_ULONGLONG(x) x##ULL

#define CNR_INT64(x) CNR_LONGLONG(x)
#define CNR_UINT64(x) CNR_ULONGLONG(x)

#define SIZE_INT8 1
#define SIZE_INT16 2
#define SIZE_INT32 4
#define SIZE_INT64 8
#define SIZE_CHAR 1

namespace cnr {

const uint8_t kuint8max = ((uint8_t)0xFF);
const uint16_t kuint16max = ((uint16_t)0xFFFF);
const uint32_t kuint32max = ((uint32_t)0xFFFFFFFF);
const uint64_t kuint64max = ((uint64_t)CNR_INT64(0xFFFFFFFFFFFFFFFF));
const int8_t kint8min = ((int8_t)0x80);
const int8_t kint8max = ((int8_t)0x7F);
const int16_t kint16min = ((int16_t)0x8000);
const int16_t kint16max = ((int16_t)0x7FFF);
const int32_t kint32min = ((int32_t)0x80000000);
const int32_t kint32max = ((int32_t)0x7FFFFFFF);
const int64_t kint64min = ((int64_t)CNR_INT64(0x8000000000000000));
const int64_t kint64max = ((int64_t)CNR_INT64(0x7FFFFFFFFFFFFFFF));

#define DISALLOW_COPY(TypeName) TypeName(const TypeName&)

#define DISALLOW_ASSIGN(TypeName) void operator=(const TypeName&)

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName();                                    \
  DISALLOW_COPY_AND_ASSIGN(TypeName)

// TODO: type computation
}  // namespace cnr
#endif  // _CNR_H_201404061148_
