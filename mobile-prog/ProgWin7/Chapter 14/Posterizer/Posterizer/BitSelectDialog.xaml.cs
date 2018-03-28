using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Posterizer
{
    public partial class BitSelectDialog : UserControl
    {
        Brush selectedBrush;
        Brush normalBrush;
        TextBlock[,] txtblks = new TextBlock[3, 9];

        public event EventHandler ColorBitsChanged;

        public BitSelectDialog()
        {
            InitializeComponent();

            ColorBits = new int[3];
            ColorBits[0] = 2;
            ColorBits[1] = 2;
            ColorBits[2] = 2;

            selectedBrush = this.Resources["PhoneAccentBrush"] as Brush;
            normalBrush = this.Resources["PhoneForegroundBrush"] as Brush;
            string[] colors = { "red", "green", "blue" };

            for (int col = 0; col < 3; col++)
            {
                TextBlock txtblk = new TextBlock
                {
                    Text = colors[col],
                    FontWeight = FontWeights.Bold,
                    TextAlignment = TextAlignment.Center,
                    Margin = new Thickness(8, 2, 8, 2)
                };

                Grid.SetRow(txtblk, 0);
                Grid.SetColumn(txtblk, col);
                LayoutRoot.Children.Add(txtblk);

                for (int bit = 0; bit < 9; bit++)
                {
                    txtblk = new TextBlock
                    {
                        Text = bit.ToString(),
                        Foreground = bit == ColorBits[col] ? selectedBrush : normalBrush,
                        TextAlignment = TextAlignment.Center,
                        Padding = new Thickness(2),
                        Tag = col.ToString() + bit
                    };

                    Grid.SetRow(txtblk, bit + 1);
                    Grid.SetColumn(txtblk, col);
                    LayoutRoot.Children.Add(txtblk);

                    txtblks[col, bit] = txtblk;
                }
            }
        }

        public int[] ColorBits { protected set; get; }

        public void Initialize(int[] colorBits)
        {
            for (int clr = 0; clr < 3; clr++)
            {
                txtblks[clr, ColorBits[clr]].Foreground = normalBrush;
                ColorBits[clr] = colorBits[clr];
                txtblks[clr, ColorBits[clr]].Foreground = selectedBrush;
            }
        }

        protected override void OnManipulationStarted(ManipulationStartedEventArgs args)
        {
            if (args.OriginalSource is TextBlock)
            {
                TextBlock txtblk = args.OriginalSource as TextBlock;
                string tag = txtblk.Tag as string;

                if (tag != null && tag.Length == 2)
                {
                    int clr = Int32.Parse(tag[0].ToString());
                    int bits = Int32.Parse(tag[1].ToString());

                    if (ColorBits[clr] != bits)
                    {
                        txtblks[clr, ColorBits[clr]].Foreground = normalBrush;
                        ColorBits[clr] = bits;
                        txtblks[clr, ColorBits[clr]].Foreground = selectedBrush;

                        if (ColorBitsChanged != null)
                            ColorBitsChanged(this, EventArgs.Empty);
                    }

                    args.Complete();
                    args.Handled = true;
                }
            }
            base.OnManipulationStarted(args);
        }
    }
}
