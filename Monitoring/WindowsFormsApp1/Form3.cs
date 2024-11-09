using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using MetroFramework.Forms;
using System.Runtime.InteropServices;
using OpenHardwareMonitor.Hardware;

namespace WindowsFormsApp1
{
    public partial class Form3 : MetroForm
    {
        private float cpu;

        private float ram;

        private ulong installedMemory;

        public Form3()
        {
            InitializeComponent();
        }

        private void Form3_Load(object sender, EventArgs e)
        {
            backgroundWorker1.RunWorkerAsync();

            MEMORYSTATUSEX mEMORYSTATUSEX = new MEMORYSTATUSEX();

            if (GlobalMemoryStatusEx(mEMORYSTATUSEX))
            {
                installedMemory = mEMORYSTATUSEX.ullTotalPhys;
            }

            metroLabel10.Text = Convert.ToString(installedMemory / 1000000000) + " Гб";

            timer1.Interval = 1000;

            timer1.Start();

        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]

        private class MEMORYSTATUSEX
        {
            public uint dwLenght;
            public uint dwMemoryLength;
            public ulong ullTotalPhys;
            public ulong ullAvailPhys;
            public ulong ullTotalPageFile;
            public ulong ullAvailPageFile;
            public ulong ullTotalVirtual;
            public ulong ullAvailVirtual;
            public ulong ullAvailExtendedVirtual;

            public MEMORYSTATUSEX()
            {
               this.dwLenght = (uint)Marshal.SizeOf(typeof(MEMORYSTATUSEX));
            }
        }

        [return: MarshalAs(UnmanagedType.Bool)]
        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        static extern bool GlobalMemoryStatusEx([In, Out] MEMORYSTATUSEX lpBuffer);

        private void metroLabel2_Click(object sender, EventArgs e)
        {

        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            cpu = performanceCPU.NextValue();
            ram = performanceRAM.NextValue();

            metroProgressBar1.Value = (int)cpu;
            metroProgressBar2.Value = (int)ram;

            metroLabel2.Text = Convert.ToString(Math.Round(cpu, 1)) + " %";
            metroLabel3.Text = Convert.ToString(Math.Round(ram, 1)) + " %";

            metroLabel6.Text = Convert.ToString(Math.Round((ram / 100 * installedMemory) / 1000000000, 1)) + " Гб";
            metroLabel8.Text = Convert.ToString(Math.Round((installedMemory - ram / 100 * installedMemory) / 1000000000, 1)) + " Гб";

            chart1.Series["ЦП"].Points.AddY(cpu);
            chart1.Series["ОЗУ"].Points.AddY(ram);
        }

        private string tmpInfo = string.Empty;


        private void GetCPUTemperature()
        {
            tmpInfo = string.Empty;

            Visitor visitor = new Visitor();

            Computer computer = new Computer();
            computer.Open();
            computer.CPUEnabled = true;
            computer.Accept(visitor);

            for (int i = 0; i < computer.Hardware.Length; i++)
            {
                if (computer.Hardware[i].HardwareType == HardwareType.CPU)
                {
                    for (int j = 0; j < computer.Hardware[i].Sensors.Length; j++)
                    {
                        if (computer.Hardware[i].Sensors[j].SensorType == SensorType.Temperature)
                        {
                            tmpInfo += computer.Hardware[i].Sensors[j].Name + ": " +
                                computer.Hardware[i].Sensors[j].Value.ToString() + "\n ";
                        }
                    }
                }
            }

            richTextBox1.Text = tmpInfo;

            computer.Close();

        }

        private void button1_Click(object sender, EventArgs e)
        {
            var form1 = new Form1();
            form1.Show();
            this.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            var form2 = new Form2();
            form2.Show();
            this.Close();
        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            while (true)
            {
                GetCPUTemperature();

                Thread.Sleep(100);
            }
        }
    }
}
