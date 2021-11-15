# TOF-wall

## Useful commands

### Open WaveDAQ Oscilloscope

Open the linux shell and run the following commands:

| Command | Description |
| --- | --- |
| `ssh foot@footon -YC`                       | *Footon* connection (with graphical interface)|
| `password`                                              | Insert password for footon|
| `cd /git/wavedaq_main/sw/wd_cl/`| path for *system.xml*|
| `wds`                                                        | Run **wds** (from anywhere)|
| `foot8080`                                              | Link to search on the browser|
| `wdx`                                                        | Select the Wavedream*x*|


### Work on WaveDAQ Oscilloscope

| Button                         | Quantity | Description |
| ---                               | ---          | --- |
| **Config**                   |               | To open the *configuration panel*|
| **Trigger**                   | Level     | Set the trigger value (i.e.to -50 mV)|
| **Trigger**                   | Delay    | Set the delay value (i.e.to 0 ns)|
| **Sampling Speed**   |              | Set the sampling rate (i.e. to 3 Gigasamples)
| **Analog Front-end** |Gain       | Set the gain (i.e. to 10)|


### Edit the configuration file

| Command | Description |
| --- | --- |
| `emacs fileconf.xlm`  | Open the configuration file with Emacs|
| `Ctrl-x Ctrl-s`            |Save the changes to the file|
| `Ctrl-x Ctrl-c`            | Quit Emacs|


### Data acquisition

| Command | Description |
| --- | --- |
| `wd_cl fileconf.xlm`  | Launch the configuration file |
| `cd/…/wd_cl`                  | Path to go to and launch **root**|


### Data analysis with ROOT

| Command | Description |
| --- | --- |
| `root -l`  | Start root session |
| `.L scripts/read_binary.C`                                                                                              | Upload the file *scripts/read_binary.C* to root |
| `decode(“nome_file.bin”)`                                                                                                | File decoding and conversion to .root |
| `.q`                                                                                                                                            | Close the root session |
| `root -l file.root`                                                                                                             | Run the generated .root file |
| `rec->MakeClass()`                                                                                                               | Run the generated .root file |
| `new TBrowser`                                                                                                                       | Open the Root Browser |
| `rec->Draw(“board166.waveform[2]”, “Entry$==0”)`                                                 | Histogram of the waveforms of ch2 |
| `rec->Draw(“board166.waveform[2]:board166.time[2]”, “Entry$==0”, “L”)`    | Plot the waveform signal as a function of time (connect the points with an "L" line) |


### Bringing footon files to my computer

| Command | Description |
| ---                                                                    | --- |
| `scp file.root root@pcfoot:[path]`     | copy a file with scp (you are on *footon* and on the folder where the file is) |
| `password`                                                      | Insert password for footon |
| `(pcfoot) mv file.root /Desktop`        | Move the file from home/Desktop |




### Small suggestions

1. Remember to insert `/` at the beginning for the path. Example: `/home/Desktop/...`. Otherwise, it cannot find the folder.

2. If you have to copy a lot of files, you should do as follows:
    - create a new folder in which to move all the files of interest:  `mkdir <folder>`
    - `mv <file*.bin> <folder>`
    - copy the entire folder with the command scp: `scp - r <folder> root@pcfoot:[path]`. 

## Open a folder from the shell

If I need to move the data files from the pc to the Google Drive (FOOT), it is necessary to open the GUI folder window and drag the files directly in the Browser.

The useful command to open the window is:

`xdg-open [folder]`

Example:

If I am in  `folder` and the files are saved in a subfolder called   `data_folder`, the only thing that I need to do is:

`xdg-open data_folder`.




