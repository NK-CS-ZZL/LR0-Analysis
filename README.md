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
<div align=center><img width = '150' height ='150' src ="https://github.com/NK-CS-ZZL/LR0-Analysis/blob/master/rules.png"/></div>
  These is the action table.<br>
<div align=center><img width = '150' height ='150' src ="https://github.com/NK-CS-ZZL/LR0-Analysis/blob/master/actions.png"/></div>

