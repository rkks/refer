using System;
using System.Collections.Generic;
using System.Reflection;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Media;
using Microsoft.Phone.Controls;

namespace PublicClasses
{
    public partial class MainPage : PhoneApplicationPage
    {
        Brush accentBrush;

        public MainPage()
        {
            InitializeComponent();
            accentBrush = this.Resources["PhoneAccentBrush"] as Brush;

            // Get all assemblies
            List<Assembly> assemblies = new List<Assembly>();
            assemblies.Add(Assembly.Load("System.Windows"));
            assemblies.Add(Assembly.Load("Microsoft.Phone"));
            assemblies.Add(Assembly.Load("Microsoft.Phone.Controls"));
            assemblies.Add(Assembly.Load("Microsoft.Phone.Controls.Maps"));

            // Set root object (use DependencyObject for shorter list)
            Type typeRoot = typeof(object);

            // Assemble total list of public classes
            List<Type> classes = new List<Type>();
            classes.Add(typeRoot);

            foreach (Assembly assembly in assemblies)
                foreach (Type type in assembly.GetTypes())
                    if (type.IsPublic && type.IsSubclassOf(typeRoot))
                        classes.Add(type);

            // Sort those classes
            classes.Sort(TypeCompare);

            // Now put all those sorted classes into a tree structure
            ClassAndChildren rootClass = new ClassAndChildren(typeRoot);
            AddToTree(rootClass, classes);

            // Display the tree
            Display(rootClass, 0);
        }

        int TypeCompare(Type t1, Type t2)
        {
            return String.Compare(t1.Name, t2.Name);
        }

        // Recursive method
        void AddToTree(ClassAndChildren parentClass, List<Type> classes)
        {
            foreach (Type type in classes)
            {
                if (type.BaseType == parentClass.Type || 
                        (type.BaseType != null && type.BaseType.IsGenericType && !type.BaseType.IsGenericTypeDefinition &&
                            type.BaseType.GetGenericTypeDefinition() == parentClass.Type))
                {
                    ClassAndChildren subClass = new ClassAndChildren(type);
                    parentClass.SubClasses.Add(subClass);
                    AddToTree(subClass, classes);
                }
            }
        }

        // Recursive method
        void Display(ClassAndChildren parentClass, int indent)
        {
            string str1 = String.Format("{0}{1}{2}{3}",
                                        new string(' ', indent * 4),
                                        parentClass.Type.Name,
                                        parentClass.Type.IsAbstract ? " (abstract)" : "",
                                        parentClass.Type.IsSealed ? " (sealed)" : "");

            string str2 = " " + parentClass.Type.Namespace;

            TextBlock txtblk = new TextBlock();
            txtblk.Inlines.Add(str1);
            txtblk.Inlines.Add(new Run
            {
                Text = str2,
                Foreground = accentBrush
            });

            stackPanel.Children.Add(txtblk);

            foreach (ClassAndChildren child in parentClass.SubClasses)
                Display(child, indent + 1);
        }
    }
}

