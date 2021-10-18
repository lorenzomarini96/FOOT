# TOF-wall

## Useful commands

### Open WaveDAQ Oscilloscope

| Command | Description |
| --- | --- |
| `ssh foot@footon -YC`                       | *Footon* connection (with graphical interface)|
| `password`                                              | Insert password for footon|
| `cd /git/wavedaq_main/sw/wd_cl/`| path for *system.xml*|
| `wds`                                                        | Run **wds** (from anywhere)|
| `foot8080`                                              | Link to search on the browser|
| `wdx`                                                        | Select the Wavedream*x*|


### Work on WaveDAQ Oscilloscope

| Button                        | Quantity | Description |
| --- | --- | --- |
| **Config**                   |               | To open the *configuration panel*|
| **Trigger**                   | Level     | Set the trigger value (i.e.to -50 mV)|
| **Trigger**                   | Delay    | Set the delay value (i.e.to 0 ns)|
| **Sampling Speed**   |              | Set the sampling rate (i.e. to 3 Gigasamples)
| **Analog Front-end** |Gain      | Set the gain (i.e. to 10)|


### Edit the configuration file

| Command | Description |
| --- | --- |
| `emacs fileconf.xlm`  | Open the configuration file with Emacs|
| `Ctrl-x Ctrl-s`            |Save the changes to the file|
| `Ctrl-x Ctrl-c`            | Quit Emacs|


### Data acquisition

| Command | Description |
| --- | --- |
| `wd_cl fileconf.xlm`  | Launch the configuration file|
| `[2]`                                 |Prepare to run|
| `[3]`                                 |The acquisition starts|
| `[5]`                                 |Stop the acquisition|
| `-1`                                   |Exit|






