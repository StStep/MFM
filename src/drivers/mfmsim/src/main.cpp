#include "main.h"
#include "ParamConfig.h"

namespace MFM {

  typedef ParamConfig<> OurParamConfig;
  typedef P1Atom<OurParamConfig> OurAtom;
  typedef CoreConfig<OurAtom,OurParamConfig> OurCoreConfig;
  typedef GridConfig<OurCoreConfig,5,3> OurGridConfig;
  struct MFMSimDHSDemo : public AbstractDriver<OurGridConfig>
  {
    MFMSimDHSDemo(DriverArguments& args) : AbstractDriver(args) { }

    virtual void ReinitPhysics() {
      OurGrid & mainGrid = GetGrid();

      mainGrid.Needed(Element_Dreg<OurCoreConfig>::THE_INSTANCE);
      mainGrid.Needed(Element_Res<OurCoreConfig>::THE_INSTANCE);
      mainGrid.Needed(Element_Sorter<OurCoreConfig>::THE_INSTANCE);
      mainGrid.Needed(Element_Emitter<OurCoreConfig>::THE_INSTANCE);
      mainGrid.Needed(Element_Consumer<OurCoreConfig>::THE_INSTANCE);
      mainGrid.Needed(Element_Data<OurCoreConfig>::THE_INSTANCE);
      mainGrid.Needed(Element_Wall<OurCoreConfig>::THE_INSTANCE);
    }

    void ReinitEden() 
    {
      OurGrid & mainGrid = GetGrid();
      StatsRenderer & srend = GetStatsRenderer();

      OurAtom atom(Element_Dreg<OurCoreConfig>::THE_INSTANCE.GetDefaultAtom());
      OurAtom sorter(Element_Sorter<OurCoreConfig>::THE_INSTANCE.GetDefaultAtom());
      OurAtom emtr(Element_Emitter<OurCoreConfig>::THE_INSTANCE.GetDefaultAtom());
      OurAtom cnsr(Element_Consumer<OurCoreConfig>::THE_INSTANCE.GetDefaultAtom());

      srend.DisplayStatsForType(Element_Empty<OurCoreConfig>::TYPE);
      srend.DisplayStatsForType(Element_Dreg<OurCoreConfig>::TYPE);
      srend.DisplayStatsForType(Element_Res<OurCoreConfig>::TYPE);
      srend.DisplayStatsForType(Element_Sorter<OurCoreConfig>::TYPE);
      srend.DisplayStatsForType(Element_Emitter<OurCoreConfig>::TYPE);
      srend.DisplayStatsForType(Element_Consumer<OurCoreConfig>::TYPE);
      srend.DisplayStatsForType(Element_Data<OurCoreConfig>::TYPE);

      emtr.SetStateField(0,10,10);  // What is this for??
      cnsr.SetStateField(0,10,10);  // What is this for??

      sorter.SetStateField(0,32,50);  // Default threshold

      u32 realWidth = P::TILE_WIDTH - P::EVENT_WINDOW_RADIUS * 2;

      SPoint aloc(20, 30);
      SPoint sloc(20, 10);
      SPoint eloc(GRID_WIDTH*realWidth-2, 10);
      SPoint cloc(1, 10);

      for(u32 x = 0; x < mainGrid.GetWidth(); x++)
        {
          for(u32 y = 0; y < mainGrid.GetHeight(); y++)
            {
              for(u32 z = 0; z < 4; z++)
                {
                  aloc.Set(20 + x * realWidth + z, 20 + y * realWidth);
                  sloc.Set(21 + x * realWidth + z, 21 + y * realWidth);
                  mainGrid.PlaceAtom(sorter, sloc);
                  mainGrid.PlaceAtom(atom, aloc);
                }
            }
        }

      mainGrid.PlaceAtom(emtr, eloc);
      mainGrid.PlaceAtom(cnsr, cloc);
      mainGrid.PlaceAtom(cnsr, cloc+SPoint(1,1));  // More consumers than emitters!
    }

  };
}

int main(int argc, char** argv)
{
  MFM::DriverArguments args(argc,argv);

  MFM::MFMSimDHSDemo sim(args);

  sim.Reinit();

  sim.Run();

  return 0;
}

