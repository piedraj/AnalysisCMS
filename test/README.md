# HistogramReader.C

These are the current functionalities of the HistogramReader.C class. The most basic one is drawing a stack of MC processes normalized by their respective cross sections.

    plotter.SetStackOption("hist");

It can also compare the shapes of different MC processes. In this case all of them are normalized to one.

    plotter.SetStackOption("nostack,hist");

Sometimes we want to see, for every process, how many events are drawn.

    plotter.SetDrawYield(true);

As an additional option, data points can be compared to the MC distributions.

    plotter.AddData("01_Data", "data", kBlack);

When the data points are available, a data / MC ratio can be drawn.

    plotter.SetDrawRatio(true);

