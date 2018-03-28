using System;
using System.Windows;
using System.Windows.Controls;

namespace Petzold.Phone.Silverlight
{
public partial class ColorColumn : UserControl
{
    public static readonly DependencyProperty ValueProperty =
        DependencyProperty.Register("Value",
            typeof(byte),
            typeof(ColorColumn),
            new PropertyMetadata((byte)0, OnValueChanged));

    public static readonly DependencyProperty LabelProperty =
        DependencyProperty.Register("Label",
            typeof(string),
            typeof(ColorColumn),
            new PropertyMetadata(OnLabelChanged));

    public event RoutedPropertyChangedEventHandler<byte> ValueChanged;

    public ColorColumn()
    {
        InitializeComponent();
    }

    public byte Value
    {
        set { SetValue(ValueProperty, value); }
        get { return (byte)GetValue(ValueProperty); }
    }

    public string Label
    {
        set { SetValue(LabelProperty, value); }
        get { return (string)GetValue(LabelProperty); }
    }

    void OnSliderValueChanged(object sender, 
                              RoutedPropertyChangedEventArgs<double> args)
    {
        Value = (byte)args.NewValue;   
    }

    static void OnValueChanged(DependencyObject obj, 
                               DependencyPropertyChangedEventArgs args)
    {
        (obj as ColorColumn).OnValueChanged((byte)args.OldValue, (byte)args.NewValue);
    }

    protected virtual void OnValueChanged(byte oldValue, byte newValue)
    {
        slider.Value = newValue;
        colorValue.Text = newValue.ToString("X2");

        if (ValueChanged != null)
            ValueChanged(this, 
                new RoutedPropertyChangedEventArgs<byte>(oldValue, newValue));
    }

    static void OnLabelChanged(DependencyObject obj, 
                               DependencyPropertyChangedEventArgs args)
    {
        (obj as ColorColumn).colorLabel.Text = args.NewValue as string;
    }
}
}
