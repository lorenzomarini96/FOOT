# Data analysis of TOFwall detector with ROOT

- Once the data acquisition is finished, a file in ```.bin``` format will be created.
- Before it can be read and manipulated for analysis, it must be converted to a ROOT file.
- This magic is accomplished by a script, called ```read_binary.C```.
- This macro implements a function called ```decode()``` to be applied directly to the affected file.

| Command | Description |
| --- | --- |
| `$ pwd /git/wavedaq_main/sw/wd_cl/`                                               | Correct path to launch root |
| `$ root -l`                                                                       | Start root session |
| `root[0] .L scripts/read_binary.C`                                                | Upload the file *scripts/read_binary.C* to root |
| `root[1] decode("name_file.bin”)`                                                 | File decoding and conversion to .root |
| `root[2] .q`                                                                      | Close the root session |
| `$ root -l file.root`                                                             | Run the generated .root file |
| `root[0] rec->MakeClass()`                                                        | created a new macro that works on any tree |
| `root[1] new TBrowser`                                                            | Open the Root Browser |
| `root[2] rec->Draw(“board166.waveform[2]”, “Entry$==0”)`                          | Histogram of the waveforms of ch2 |
| `root[3] rec->Draw(“board166.waveform[2]:board166.time[2]”, “Entry$==0”, “L”)`    | Plot the waveform signal as a function of time (connect the points with an "L" line) |



