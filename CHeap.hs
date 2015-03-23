module CHeap where

import Foreign
import Foreign.C.Types

data Heap

foreign import ccall unsafe "realtime_dynamic_int_heap.h heap_alloc"
  c_heap_alloc :: IO (Ptr Heap)

foreign import ccall unsafe "realtime_dynamic_int_heap.h heap_free"
  c_heap_free :: Ptr Heap -> IO ()

foreign import ccall unsafe "realtime_dynamic_int_heap.h heap_size"
  c_heap_size :: Ptr Heap -> CSize

foreign import ccall unsafe "realtime_dynamic_int_heap.h heap_is_empty"
  c_heap_is_empty :: Ptr Heap -> Bool

foreign import ccall unsafe "realtime_dynamic_int_heap.h heap_insert"
  c_heap_insert :: Ptr Heap -> CInt -> IO ()

foreign import ccall unsafe "realtime_dynamic_int_heap.h heap_replace"
  c_heap_replace :: Ptr Heap -> CInt -> IO ()

foreign import ccall unsafe "realtime_dynamic_int_heap.h heap_max"
  c_heap_max :: Ptr Heap -> IO CInt

foreign import ccall unsafe "realtime_dynamic_int_heap.h heap_extract_max"
  c_heap_extract_max :: Ptr Heap -> IO CInt

foreign import ccall unsafe "realtime_dynamic_int_heap.h heap_increase_key"
  c_heap_increase_key :: Ptr Heap -> IO ()

foreign import ccall unsafe "realtime_dynamic_int_heap.h heap_delete"
  c_heap_delete :: Ptr Heap -> CSize -> IO ()

foreign import ccall unsafe "realtime_dynamic_int_heap.h heapify"
  c_heapify:: Ptr CInt -> CSize -> IO ()

foreign import ccall unsafe "realtime_dynamic_int_heap.h heap_sort"
  c_heap_sort:: Ptr CInt -> CSize -> IO ()

foreign import ccall unsafe "realtime_dynamic_int_heap.h heap_from_array"
  c_heap_from_array:: Ptr CInt -> CSize -> IO (Ptr Heap)

foreign import ccall unsafe "realtime_dynamic_int_heap.h heap_degenerate_to_sorted_array"
  c_heap_degenerate_to_sorted_array:: Ptr Heap -> Ptr CSize -> IO (Ptr CInt)
