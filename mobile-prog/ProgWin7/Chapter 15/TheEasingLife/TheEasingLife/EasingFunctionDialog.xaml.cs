using System;
using System.Reflection;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Animation;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;

namespace TheEasingLife
{
    public partial class EasingFunctionDialog : PhoneApplicationPage
    {
        public EasingFunctionDialog()
        {
            InitializeComponent();
        }

        public EasingFunctionBase EasingFunction { get; set; }

        void OnRadioButtonChecked(object sender, RoutedEventArgs args)
        {
            Type type = (sender as RadioButton).Tag as Type;

            if (type == null)
            {
                EasingFunction = null;
            }
            else
            {
                ConstructorInfo constructor = type.GetConstructor(Type.EmptyTypes);
                EasingFunction = constructor.Invoke(null) as EasingFunctionBase;
            }
        }

        protected override void OnNavigatedTo(NavigationEventArgs args)
        {
            // Create "None" RadioButton
            RadioButton radio = new RadioButton();
            radio.Content = "None";
            radio.IsChecked = (EasingFunction == null);
            radio.Checked += OnRadioButtonChecked;
            stack.Children.Add(radio);

            Assembly assembly = Assembly.Load("System.Windows");

            // Create RadioButton for each easing function
            foreach (Type type in assembly.GetTypes())
                if (type.IsPublic && type.IsSubclassOf(typeof(EasingFunctionBase)))
                {
                    radio = new RadioButton();
                    radio.Content = type.Name;
                    radio.Tag = type;
                    radio.IsChecked = (EasingFunction != null && 
                                        EasingFunction.GetType() == type);
                    radio.Checked += OnRadioButtonChecked;
                    stack.Children.Add(radio);
                }

            base.OnNavigatedTo(args);
        }

        protected override void OnNavigatedFrom(NavigationEventArgs args)
        {
            if (args.Content is MainPage)
            {
                (args.Content as MainPage).EasingFunction = EasingFunction;
            }

            base.OnNavigatedFrom(args);
        }
    }
}