using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AIProject1
{
    class Tree
    {
        public Tree()
        {
            nodeList = new List<Node>();
        }

        public Node root { get; set; }
        public List<Node> nodeList { get; set; }
    }
}
