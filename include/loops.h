/*
** loops for PSU_2015_philo
**
** Made by pirou_g in /home/pirou_g/ClionProjects/PSU_2015_philo
** Login   <pirou_g@epitech.net>
**
** Started on     08/03/16 18:35
** Last update on 08/03/16 18:35
*/

#ifndef LOOPS_H_
# define LOOPS_H_

# include <setjmp.h>

# define DO {jmp_buf do_buf; setjmp(do_buf);
# define WHILE(x) if (x) {longjmp(do_buf, 1);}}

# define PART_FOR jmp_buf fb; int started = 0; setjmp(fb);
# define FOR(i, cdt, up) {i; PART_FOR if (started) {up;} if (cdt) {started = 1;
# define END_FOR longjmp(fb, 1); }}

#endif /* LOOPS_H_ */
