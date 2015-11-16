# HistogramReader.C

These are the current functionalities of the HistogramReader.C class:

1. The most basic one is drawing a stack of MC processes normalized by their respective cross sections.

    plotter.SetStackOption("hist");

2. It can also compare the shapes of different MC processes. In this case all of them are normalized to one.

     plotter.SetStackOption("nostack,hist");

3. Sometimes we want to see, for every process, how many events are drawn.

     plotter.SetDrawYield(true);

4. As an additional option, data points can be compared to the MC distributions.

     plotter.AddData("01_Data", "data", kBlack);

5. When the data points are available, a data / MC ratio can be drawn.

    plotter.SetDrawRatio (true);
