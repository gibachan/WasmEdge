(module
  (memory (import "js" "mem") 1)
  (func (export "accumulate") (param $ptr i32) (param $len i32) (result i32)
    (i32.store (i32.const 1) (i32.const 2))
    (i32.load (i32.const 0))
  )
)