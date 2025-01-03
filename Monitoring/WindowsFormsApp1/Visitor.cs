﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenHardwareMonitor.Hardware;

namespace WindowsFormsApp1
{
    internal class Visitor : IVisitor
    {
        public void VisitComputer(IComputer computer)
        {
            computer.Traverse(this);
        }

        public void VisitHardware(IHardware hardware)
        {
            hardware.Update();

            foreach (IHardware hw in hardware.SubHardware)
            {
                hw.Accept(this);
            }
        }

        public void VisitParameter(IParameter parameter)
        {
            throw new NotImplementedException();
        }

        public void VisitSensor(ISensor sensor)
        {
            throw new NotImplementedException();
        }
    }
}
