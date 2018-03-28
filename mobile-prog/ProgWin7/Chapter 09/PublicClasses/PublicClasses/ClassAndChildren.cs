using System;
using System.Collections.Generic;

namespace PublicClasses
{
    class ClassAndChildren
    {
        public ClassAndChildren(Type parent)
        {
            Type = parent;
            SubClasses = new List<ClassAndChildren>();
        }

        public Type Type { set; get; }
        public List<ClassAndChildren> SubClasses { set; get; }
    }
}