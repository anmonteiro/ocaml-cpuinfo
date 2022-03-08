#include "num_cores.h"
#define CAML_NAME_SPACE
#include <caml/alloc.h>
#include <caml/custom.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

CAMLprim value ocaml_get_numcores(value unit) {
  CAMLparam0();
  uint32_t n = getNumberOfProcessors();
  CAMLreturn(Val_int(n));
}
