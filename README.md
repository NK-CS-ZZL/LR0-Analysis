# LR0-Analysis
## Arithmetic and logical expression evaluation based on LR(0) Analysis
&emsp;&emsp;After studying "compile principle", I decided to make a demo as execrise, and this is product of my labour-a parser which has 6 priority levels and supports arithmetic and logic expression.<br>
&emsp;&emsp;I simplify the priority levels in C, such as '==' and '-=' oughtn't to have the same priority, for it's too exhausting to analyse a syntax with so many rules. After simplication, priority table is the following one.<br>

 | 优先级 | 符号 |
 |:----:|:----:|
 |  1  |unary op|
 |  2  | *, / |
 |  3  | +, - |
 |  4  | compare op|
 |  5  |  &&  |
 |  6  | \|\| |

 &emsp;&emsp;And these are rules of syntax.<br>
![Rules](https://github.com/NK-CS-ZZL/LR0-Analysis/blob/master/rules.png)<br>
&emsp;&emsp;These is the action table.Though it has quantities of states, you can see that it's incredibly regular. So if you think it's too hard to establish this table, you can try a less-priority-levels one. For example, a table including four arithmetic operations and two priority is a good idea, and this is how I finish this work.<br>
![Actions](https://github.com/NK-CS-ZZL/LR0-Analysis/blob/master/actions.png)<br>
&emsp;&emsp;In above table, postive value means shift action, negative value means reduce action and zero means no action(if you get a zero, that means reduction error occurs). You can find more instances in comments in syntax.cpp.<br>
&emsp;&emsp;This program can only support integer calculation. However, it isn't difficult to be compatible with float. You just need  convert functions between string and float.<br> 
&emsp;&emsp;I attempt to take all edge cases into account, such as overflow, underflow, divide zero, but maybe there are still some bugs.<br>
&emsp;&emsp;If you intend to gain deeper knowledge about LR analysis, I hope this program may help you.





