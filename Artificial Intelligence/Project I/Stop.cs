using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AIProject1
{
    class Stop
    {

        public void Print()
        {
            Console.WriteLine("Id: " + id + "\tName: " + name);
        }

        public int id { get; set; }
        public double latitude { get; set; }
        public double longitude { get; set; }
        public String name { get; set; }
    }
}
