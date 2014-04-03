using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AIProject1
{
    class Route
    {
        public Route()
        {
            stops = new List<Stop>();
        }

        public void Print()
        {
            Console.WriteLine("Id: " + id + "\tName: " + name);
            Console.WriteLine("Stops:");
            foreach (Stop stop in stops)
            {
                Console.WriteLine(stop.name + " ");
            }
            Console.WriteLine();
        }

        public int id { get; set; }
        public String name { get; set; }
        public List<Stop> stops { get; set; }
    }
}
