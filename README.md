
Solver for [SortPuz mobile game](https://play.google.com/store/apps/details?id=sortpuz.water.sort.puzzle.game&hl=ru&gl=US), written in C++.     

Algorithm is optimized bruteforce, which uses cost function to analyze which states are more likely to lead to an answer, and bruteforce visits them in the first place..

To run it, compile the ``main.cpp`` file youself or ask some programmer you know to do it. Then run the compiled binary and use your game field as input in terminal. You can deduce it's format from the following picture and example, color can be named as you want.  

<img src="https://i.imgur.com/w6QmYit.jpg" alt="drawing" width="300"/>                                                  

Corresponing input (where 7 is number of bottles): 
`````
7
blue yellow yellow pink end
blue violet violet end
pink blue end
violet pink pink yellow end
yellow orange violet blue end
orange orange orange end
end
`````
Notice that if first line is $n$ (meaning you have $n$ bottles), it should be followed by exactly $n$ lines, each ending with "end". 
You can name colors arbitrarily, e.g. the following for the same picture should also work:
`````
7
a b b c end
a b b end
c a end
b c c b end
b d b a end
d d d end
end
`````

Output will be a lots of spam for tracking progress and then a sequence of lines in "from which bottle to which bottle" format. Bottles are numbered as follows:
 
<img src="https://i.imgur.com/BM52qqO.jpg" alt="drawing" width="300"/>         

For example, the output for configuration higher will be
1 7
4 1
4 7
2 4
2 3
1 2
... and a lot of more, meaning at first you should transfer liquid from bottle 1 to bottle 7, then from bottle 4 to number 1, then from bottle 4 to number 7, and so on.
You get it. Good luck!


