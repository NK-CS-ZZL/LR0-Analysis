# LR0-Analysis
## Arithmetic and logical expression evaluation based on LR(0) Analysis
  After studying "compile principle", I decided to make a demo as execrise, and this is product of my labour-a parser which has 6 priority
levels and supports arithmetic and logic expression.<br>
  I simplify the priority levels in C, such as '==' and '-=' oughtn't to have the same priority, for it's too exhausting to analyse a syntax
 with so many rules. After simplication, priority table is the following one.<br>
 | 优先级 | 符号 |
 |:----:|:----:|
 |  1  |unary op|
 |  2  | *, / |
 |  3  | +, - |
 |  4  | compare op|
 |  5  |  &&  |
 |  6  | \|\| |

  And these are rules of syntax.<br>
![Rules](https://github.com/NK-CS-ZZL/LR0-Analysis/blob/master/rules.png)
  These is the action table.<br>
![Actions]("https://github.com/NK-CS-ZZL/LR0-Analysis/blob/master/actions.png")
  This program can only support integer calculation. However, it isn't difficult to be compatible with float. You just need  convert functions
  between string and float.<br> 
   I attempt to take all edge cases into account, such as overflow, underflow, divide zero, but maybe there are still some bugs.<br>
   If you intend to gain deeper knowledge about LR analysis, I hope this program may help you.





