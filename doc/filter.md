
# Low Pass Filter

$innovation = new_meas - prev_meas$

We want to prevent large changes

$new_meas = prev_meas + 1/\Tau(new_meas - prev_meas)$
