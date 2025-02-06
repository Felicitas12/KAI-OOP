using System;
using System.Linq;
using System.Runtime.InteropServices;

public class Lines
{
    private string value;

    public Lines(string input)
    {
        value = input;
    }

    public string GetValue()
    {
        return value;
    }

    public int GetLength()
    {
        return value.Length;
    }

    public string SortAscending()
    {
        value = new string(value.OrderBy(c => c).ToArray());
        return value;
    }

    public static Lines CreateInstance(string input)
    {
        return new Lines(input);
    }
}
