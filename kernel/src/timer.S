.section .text

.global timer_physical_frequency
.type timer_physical_frequency, @function
timer_physical_frequency:
  mrs x0, cntfrq_el0
  ret

.global timer_physical_get_value
.type timer_physical_get_value, @function
timer_physical_get_value:
  mrs x0, cntpct_el0
  ret

.global timer_physical_enable
.type timer_physical_enable, @function
timer_physical_enable:
  mov x1, 1
  msr cntp_ctl_el0, x1
  ret

.global timer_physical_set_threshold
.type timer_physical_set_threshold, @function
timer_physical_set_threshold:
  msr cntp_cval_el0, x0
  ret

.global timer_physical_add_threshold
.type timer_physical_add_threshold, @function
timer_physical_add_threshold:
  msr cntp_tval_el0, x0
  ret

.global timer_physical_time_passed
.type timer_physical_time_passed, @function
timer_physical_time_passed:
  mrs x0, cntp_tval_el0
  ret

