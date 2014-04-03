using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AIProject1
{
    class Node
    {
        public Node()
        {
            Parent = null;
            Child = new List<Node>();
            isRoot = false;
            value = null;
        }

        public void AddChild(Stop stop, Tree tree)
        {
            Node node = new Node();
            node.Parent = this;
            node.value = stop;
            Child.Add(node);
            tree.nodeList.Add(node);
        }

        public Stop value { get; set; }
        public bool isRoot { get; set; }
        public Node Parent { get; set; }
        public List<Node> Child { get; set; }
    }
}
