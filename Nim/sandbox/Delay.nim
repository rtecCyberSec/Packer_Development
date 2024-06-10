import winim
import os, random, times

proc randomCalculations(duration: int) =
  let startTime = GetTickCount()
  while (GetTickCount() - startTime) < duration:
    let a = rand(1..100)
    let b = rand(1..100)
    discard a + b  # Do a random calculation
