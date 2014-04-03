using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace AIProject1
{
    class Program
    {
        static void Main(string[] args)
        {
            // read from files
            List<Route> routes = new List<Route>();
            List<Stop> stops = new List<Stop>();
            readRoutes(ref routes);
            readStops(ref stops);
            readRouteStops(ref routes, stops);
         

           /* routes[241].Print();
            Console.ReadKey();

            routes[199].Print();
            Console.ReadKey();*/

            //bfsGraph(routes, stops);
            //Console.ReadKey();
            Console.WriteLine();

            dfsGraph(routes, stops);
            Console.ReadKey();

        }

        public static void readRoutes(ref List<Route> routes)
        {
            var reader = new StreamReader(File.OpenRead(@"routes.csv"));
            while (!reader.EndOfStream)
            {
                var line = reader.ReadLine();
                var values = line.Split(',');
                Route route = new Route();
                route.id = Int32.Parse(values[0]);
                route.name = values[1];
                routes.Add(route);
            }
        }

        private static void readStops(ref List<Stop> stops)
        {
            var reader = new StreamReader(File.OpenRead(@"stops.csv"));
            while (!reader.EndOfStream)
            {
                var line = reader.ReadLine();
                var values = line.Split(',');
                Stop stop = new Stop();
                stop.id = Int32.Parse(values[0]);
                stop.latitude = Double.Parse(values[1]);
                stop.longitude = Double.Parse(values[2]);
                stop.name = values[3];
                stops.Add(stop);
            }
        }

        private static void readRouteStops(ref List<Route> routes, List<Stop> stops)
        {
            var reader = new StreamReader(File.OpenRead(@"route_stops.csv"));
            while (!reader.EndOfStream)
            {
                var line = reader.ReadLine();
                var values = line.Split(',');
                routes[Int32.Parse(values[0])-1].stops.Add(stops[Int32.Parse(values[1])-1]);
            }
        }

        private static void bfsTree(List<Route> routes, List<Stop> stops)
        {
            Stop initial = stops.FirstOrDefault(o => o.name == "BAKIRKÖY");
            Stop goal = stops.FirstOrDefault(o => o.name == "KABATAŞ");
           /* if (goal != null)
                goal.Print(); 
              if(initial != null)
                  initial.Print();  */

            // prepare the resulting tree which will be used to create solutioin sequence
            Tree solutionTree = new Tree();

            // find routes passing through a stop
            List<Route> passRoutes = new List<Route>(); 
            //routes.Where(o => o.stops.Contains(initial)).ToList().ForEach(o => passRoutes.Add(o));

            Queue<Stop> frontier = new Queue<Stop>(); frontier.Enqueue(initial);

            while (true)
            {
                if (frontier.Count == 0)
                {
                    Console.WriteLine("No Solution");
                    break;
                }

                Stop currentStop = frontier.Dequeue();
                passRoutes.Clear();
                routes.Where(o => o.stops.Contains(currentStop)).ToList().ForEach(o => passRoutes.Add(o));

                Node node = new Node();
               
                if (currentStop.name == goal.name)
                {
                    Console.WriteLine("Destination Reached.");
                    node = solutionTree.nodeList.FirstOrDefault(o => o.value.name == currentStop.name);
                    if (node == null)
                        Console.WriteLine("Current Node not found in the solution tree node list: " + currentStop.name);
                    List<Stop> solution = new List<Stop>();
                    while (node.isRoot == false)
                    {
                        solution.Add(node.value);
                        node = node.Parent;
                    }
                    solution.Add(node.value);

                    solution.Reverse();
                    Console.WriteLine("\nSolution is: \n");
                    Console.ReadKey();     /*
                    foreach (Stop s in solution)
                    {
                        Console.Write(s.name + "  ");
                        Console.WriteLine();
                    }
                                  */
                    Stop stop1; Stop stop2;
                    for (int i = 0; i < solution.Count - 1; i++)
                    {
                        stop1 = solution[i];
                        stop2 = solution[i + 1];
                        Route r = findRoute(stop1, stop2, routes);
                        Console.WriteLine(stop1.name + " - " + stop2.name + " => " + r.name);
                    }

                    break;
                }

                // if tree is initially empty, add the root node
                if (solutionTree.nodeList.Count == 0)
                {
                    node.value = currentStop;
                    node.isRoot = true;
                    solutionTree.root = node;
                    solutionTree.nodeList.Add(node);
                }

                else
                {
                    node = solutionTree.nodeList.FirstOrDefault(o => o.value.id == currentStop.id);
                    if (node == null)
                        Console.WriteLine("Current Node not found in the solution tree node list: " + currentStop.name);
                }



                //Console.WriteLine("Routes passing through: " + currentStop.name);
                // find adjacent stops of the current stop
                foreach (Route r in passRoutes) // for each route containing current stop
                {
                    //Console.Write(r.name + " :\t");
                    // find the index of the current stop in the routes containing the stop
                    int index = r.stops.IndexOf(currentStop);
                    if (index >= 1)  // if its not the first element
                    {
                        // add frontier and tree the previous stop
                        frontier.Enqueue(r.stops[index - 1]);
                        node.AddChild(r.stops[index - 1], solutionTree);
                        //Console.Write(r.stops[index - 1].name + ", ");

                        if (index < r.stops.Count - 1)  // and not the last element
                        {
                            // add frontier the next stop in the list
                            frontier.Enqueue(r.stops[index + 1]);
                            node.AddChild(r.stops[index + 1], solutionTree);
                            //Console.Write(r.stops[index + 1].name + " ");
                        }
                    }
                    else  // if its the first element
                    {
                        if (index < r.stops.Count - 1)  // and not the last element
                        {
                            // add frontier the next stop in the list
                            frontier.Enqueue(r.stops[index + 1]);
                            node.AddChild(r.stops[index + 1], solutionTree);
                            //Console.Write(r.stops[index + 1].name + " ");
                        }
                    }
                    //Console.WriteLine();
                }

                //Console.ReadKey();
            }

        }

        private static void bfsGraph(List<Route> routes, List<Stop> stops)
        {
            Stop initial = stops.FirstOrDefault(o => o.name == "BAKIRKÖY");
            Stop goal = stops.FirstOrDefault(o => o.name == "KABATAŞ");

            // prepare the resulting tree which will be used to create solutioin sequence
            Tree solutionTree = new Tree();

            // find routes passing through a stop
            List<Route> passRoutes = new List<Route>();

            Queue<Stop> frontier = new Queue<Stop>(); frontier.Enqueue(initial);
            List<Stop> explored = new List<Stop>();

            while (true)
            {
                if (frontier.Count == 0)
                {
                    Console.WriteLine("No Solution");
                    break;
                }

                Stop currentStop = frontier.Dequeue();
                passRoutes.Clear();
                routes.Where(o => o.stops.Contains(currentStop)).ToList().ForEach(o => passRoutes.Add(o));

                explored.Add(currentStop);

                Node node = new Node();

                if (currentStop.name == goal.name)
                {
                    Console.WriteLine("Destination Reached.");
                    node = solutionTree.nodeList.FirstOrDefault(o => o.value.name == currentStop.name);
                    if (node == null)
                        Console.WriteLine("Current Node not found in the solution tree node list: " + currentStop.name);
                    List<Stop> solution = new List<Stop>();
                    while (node.isRoot == false)
                    {
                        solution.Add(node.value);
                        node = node.Parent;
                    }
                    solution.Add(node.value);

                    solution.Reverse();
                    Console.WriteLine("\nSolution is: \n");
                    Console.ReadKey();     /*
                    foreach (Stop s in solution)
                    {
                        Console.Write(s.name + "  ");
                        Console.WriteLine();
                    }
                                  */
                    Stop stop1; Stop stop2;
                    for (int i = 0; i < solution.Count-1; i++)
                    {
                        stop1 = solution[i];
                        stop2 = solution[i + 1];
                        Route r = findRoute(stop1, stop2, routes);
                        Console.WriteLine(stop1.name + " - " + stop2.name + " => " + r.name);
                    }
                    
                    break;                
                }


                // if tree is initially empty, add the root node
                if (solutionTree.nodeList.Count == 0)
                {
                    node.value = currentStop;
                    node.isRoot = true;
                    solutionTree.root = node;
                    solutionTree.nodeList.Add(node);
                } else
                {
                    node = solutionTree.nodeList.FirstOrDefault(o => o.value.name == currentStop.name);
                    if (node == null)
                        Console.WriteLine("Current Node not found in the solution tree node list: " + currentStop.name);
                }

                //Console.WriteLine("Routes passing through: " + currentStop.name);  
                
                // find adjacent stops of the current stop
                foreach (Route r in passRoutes) // for each route containing current stop
                {
                    //Console.Write(r.name + " :\t");
                    // find the index of the current stop in the routes containing the stop
                    int index = r.stops.IndexOf(currentStop);
                    if (index >= 1)  // if its not the first element
                    {
                        // add frontier and tree the previous stop
                        if (frontier.Contains(r.stops[index - 1]) == false &&
                            explored.Contains(r.stops[index - 1]) == false)
                        {
                            frontier.Enqueue(r.stops[index - 1]);
                            node.AddChild(r.stops[index - 1], solutionTree);
                            //Console.Write(r.stops[index - 1].name + ", ");
                        }
                       
                    }
                    if (index < r.stops.Count - 1)  // and not the last element
                    {
                        if (frontier.Contains(r.stops[index + 1]) == false &&
                        explored.Contains(r.stops[index + 1]) == false)
                        {
                            // add frontier the next stop in the list
                            frontier.Enqueue(r.stops[index + 1]);
                            node.AddChild(r.stops[index + 1], solutionTree);
                            //Console.Write(r.stops[index + 1].name + " ");
                        }
                    }
                    //Console.WriteLine();
                }

                //Console.ReadKey();
            }

        }

        private static Route findRoute(Stop s1, Stop s2, List<Route> routes)
        {
            Route ans = null;
            List<Route> passRoutes = new List<Route>();
            routes.Where(o => o.stops.Contains(s1)).ToList().ForEach(o => passRoutes.Add(o));
            ans = passRoutes.FirstOrDefault(o => o.stops.Contains(s2));
            return ans;
        }

        private static void dfsGraph(List<Route> routes, List<Stop> stops)
        {
            Stop initial = stops.FirstOrDefault(o => o.name == "BAKIRKÖY");
            Stop goal = stops.FirstOrDefault(o => o.name == "KABATAŞ");

            // prepare the resulting tree which will be used to create solutioin sequence
            Tree solutionTree = new Tree();

            // find routes passing through a stop
            List<Route> passRoutes = new List<Route>();

            Stack<Stop> frontier = new Stack<Stop>(); frontier.Push(initial);
            List<Stop> explored = new List<Stop>();

            while (frontier.Count != 0)
            {
                if (frontier.Count == 0)
                {
                    Console.WriteLine("No Solution");
                    break;
                }

                Stop currentStop = frontier.Peek();
                passRoutes.Clear();
                routes.Where(o => o.stops.Contains(currentStop)).ToList().ForEach(o => passRoutes.Add(o));

                Node node = new Node();

                if (currentStop.name == goal.name)
                {
                    Console.WriteLine("Destination Reached.");
                    node = solutionTree.nodeList.FirstOrDefault(o => o.value.name == currentStop.name);
                    if (node == null)
                        Console.WriteLine("Current Node not found in the solution tree node list: " + currentStop.name);
                    List<Stop> solution = new List<Stop>();
                    while (node.isRoot == false)
                    {
                        solution.Add(node.value);
                        node = node.Parent;
                    }
                    solution.Add(node.value);

                    solution.Reverse();
                    Console.WriteLine("\nSolution is: \n");
                    Console.ReadKey();     /*
                    foreach (Stop s in solution)
                    {
                        Console.Write(s.name + "  ");
                        Console.WriteLine();
                    }                       */
                                  
                    Stop stop1; Stop stop2;
                    for (int i = 0; i < solution.Count - 1; i++)
                    {
                        stop1 = solution[i];
                        stop2 = solution[i + 1];
                        Route r = findRoute(stop1, stop2, routes);
                        if( r != null)
                            Console.WriteLine(stop1.name + " - " + stop2.name + " => " + r.name);
                    }

                    break;
                }


                // if tree is initially empty, add the root node
                if (solutionTree.nodeList.Count == 0)
                {
                    node.value = currentStop;
                    node.isRoot = true;
                    solutionTree.root = node;
                    solutionTree.nodeList.Add(node);
                }
                else
                {
                    node = solutionTree.nodeList.FirstOrDefault(o => o.value.name == currentStop.name);
                    if (node == null)
                        Console.WriteLine("Current Node not found in the solution tree node list: " + currentStop.name);
                }

                //Console.WriteLine("Routes passing through: " + currentStop.name);
                //Console.ReadKey();
                // find adjacent stops of the current stop
                bool added = false;
                foreach (Route r in passRoutes) // for each route containing current stop
                {
                    //Console.Write(r.name + " :\t");
                    // find the index of the current stop in the routes containing the stop
                    int index = r.stops.IndexOf(currentStop);
                    if (index >= 1)  // if its not the first element
                    {
                        // add frontier and tree the previous stop
                        if (frontier.Contains(r.stops[index - 1]) == false &&
                            explored.Contains(r.stops[index - 1]) == false)
                        {
                            frontier.Push(r.stops[index - 1]);
                            node.AddChild(r.stops[index - 1], solutionTree);
                            added = true;
                            //Console.Write(r.stops[index - 1].name + ", ");
                        }

                    }
                    if (index < r.stops.Count - 1)  // and not the last element
                    {
                        if (frontier.Contains(r.stops[index + 1]) == false &&
                        explored.Contains(r.stops[index + 1]) == false)
                        {
                            // add frontier the next stop in the list
                            frontier.Push(r.stops[index + 1]);
                            node.AddChild(r.stops[index + 1], solutionTree);
                            added = true;
                            //Console.Write(r.stops[index + 1].name + " ");
                        }
                    }
                    //Console.WriteLine();
                }
                if (added == false)
                {
                    explored.Add(frontier.Pop());
                }
                
                
            }

        }

        private static void AStar(List<Route> routes, List<Stop> stops)
        {
            // see project report.
        }
    }
}
