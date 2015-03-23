{-# LANGUAGE TemplateHaskell #-}

module Main where

import CHeap
import Foreign hiding (void)
import Foreign.C.Types
import Test.QuickCheck
import Test.QuickCheck.Monadic
import Control.Monad (forM, forM_, void)
import System.IO.Unsafe (unsafePerformIO)
import Data.List (sort, sortBy)
import Data.List.Ordered (isSorted, isSortedBy)

data HeapPtr = HeapPtr (Ptr Heap) CSize deriving Show

instance Arbitrary CInt where
  arbitrary = choose (minBound :: CInt, maxBound :: CInt)

instance Arbitrary HeapPtr where
  arbitrary = sized $ \n -> do
    xs <- vector n
    return $ listToHeap xs

listToHeap :: [CInt] -> HeapPtr
listToHeap = unsafePerformIO . listToHeap'

listToHeap' :: [CInt] -> IO HeapPtr
listToHeap' xs = do
  h <- c_heap_alloc
  forM_ xs (c_heap_insert h)
  return (HeapPtr h (fromIntegral (length xs)))

prop_heap_preserves_input :: [CInt] -> Property
prop_heap_preserves_input xs = monadicIO $ do
  h <- run c_heap_alloc
  run $ forM_ xs (c_heap_insert h)
  xs' <- run $ forM xs (\_ -> c_heap_extract_max h)
  assert (sortBy (flip compare) xs == xs')

prop_extract_max_reverse_sorted :: HeapPtr -> Property
prop_extract_max_reverse_sorted (HeapPtr h sz) = monadicIO $ do
  xs <- run $ forM [1..sz] (\_ -> c_heap_extract_max h)
  run $ c_heap_free h
  assert (isSortedBy (>=) xs)

prop_insert_max :: NonEmptyList CInt -> Property
prop_insert_max (NonEmpty xs) = monadicIO $ do
  HeapPtr h _ <- run $ listToHeap' xs
  let m = maximum xs + 1
  run $ c_heap_insert h m
  n <- run $ c_heap_max h
  run $ c_heap_free h
  assert (m == n)

prop_heap_sort_sorts :: [CInt] -> Property
prop_heap_sort_sorts xs = monadicIO $ do
  let sorted = sort xs
      size = length xs
  ptr <- run $ newArray xs
  run $ c_heap_sort ptr (fromIntegral size)
  xs' <- run $ peekArray size ptr
  assert (xs' == sorted)

prop_degenerates_to_sorted_array :: HeapPtr -> Property
prop_degenerates_to_sorted_array (HeapPtr h sz) = monadicIO $ do
  sz_out <- run malloc
  sorted_ptr <- run $ c_heap_degenerate_to_sorted_array h sz_out
  sorted <- run $ peekArray (fromIntegral sz) sorted_ptr
  run $ free sz_out
  run $ free sorted_ptr
  assert (isSorted sorted)

return []
runTests :: IO Bool
runTests = $quickCheckAll

runDeepTests :: IO Bool
runDeepTests = $forAllProperties $ quickCheckWithResult
               (stdArgs { maxSuccess = 1000, maxSize = 1000})
main :: IO ()
main = void runDeepTests
