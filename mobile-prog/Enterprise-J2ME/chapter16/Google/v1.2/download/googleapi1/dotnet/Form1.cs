using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace Google_Web_APIs_Demo
{
	/// <summary>
	/// A demo program of the Google Web APIs, written for
	///   Microsoft Development Environment 2002 Version 7.0.9466
	///   .NET Framework SDK                     Version 1.0.3705
	/// See bottom of file for Google code examples
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox keyBox;
		private System.Windows.Forms.Button searchButton;
		private System.Windows.Forms.TextBox searchBox;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label searchResultLabel;
		private System.Windows.Forms.TextBox cacheBox;
		private System.Windows.Forms.Button cacheButton;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label cacheResultLabel;
		private System.Windows.Forms.TextBox spellBox;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Button spellButton;
		private System.Windows.Forms.Label spellResultLabel;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Form1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.label1 = new System.Windows.Forms.Label();
			this.keyBox = new System.Windows.Forms.TextBox();
			this.searchButton = new System.Windows.Forms.Button();
			this.searchBox = new System.Windows.Forms.TextBox();
			this.label2 = new System.Windows.Forms.Label();
			this.searchResultLabel = new System.Windows.Forms.Label();
			this.cacheBox = new System.Windows.Forms.TextBox();
			this.cacheButton = new System.Windows.Forms.Button();
			this.label3 = new System.Windows.Forms.Label();
			this.cacheResultLabel = new System.Windows.Forms.Label();
			this.spellBox = new System.Windows.Forms.TextBox();
			this.spellButton = new System.Windows.Forms.Button();
			this.label4 = new System.Windows.Forms.Label();
			this.spellResultLabel = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(8, 16);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(104, 16);
			this.label1.TabIndex = 0;
			this.label1.Text = "Enter license key:";
			// 
			// keyBox
			// 
			this.keyBox.Location = new System.Drawing.Point(120, 16);
			this.keyBox.Name = "keyBox";
			this.keyBox.Size = new System.Drawing.Size(240, 20);
			this.keyBox.TabIndex = 1;
			this.keyBox.Text = "xxxxxxxxxxxxxxxxxxxxxxxx";
			// 
			// searchButton
			// 
			this.searchButton.Location = new System.Drawing.Point(296, 64);
			this.searchButton.Name = "searchButton";
			this.searchButton.Size = new System.Drawing.Size(64, 24);
			this.searchButton.TabIndex = 3;
			this.searchButton.Text = "Search";
			this.searchButton.Click += new System.EventHandler(this.searchButton_Click);
			// 
			// searchBox
			// 
			this.searchBox.Location = new System.Drawing.Point(8, 64);
			this.searchBox.Name = "searchBox";
			this.searchBox.Size = new System.Drawing.Size(280, 20);
			this.searchBox.TabIndex = 4;
			this.searchBox.Text = "Enter search term";
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(8, 96);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(112, 16);
			this.label2.TabIndex = 5;
			this.label2.Text = "Est. # Results:";
			this.label2.TextAlign = System.Drawing.ContentAlignment.TopRight;
			// 
			// searchResultLabel
			// 
			this.searchResultLabel.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.searchResultLabel.Location = new System.Drawing.Point(120, 96);
			this.searchResultLabel.Name = "searchResultLabel";
			this.searchResultLabel.Size = new System.Drawing.Size(240, 16);
			this.searchResultLabel.TabIndex = 6;
			// 
			// cacheBox
			// 
			this.cacheBox.Location = new System.Drawing.Point(8, 144);
			this.cacheBox.Name = "cacheBox";
			this.cacheBox.Size = new System.Drawing.Size(280, 20);
			this.cacheBox.TabIndex = 7;
			this.cacheBox.Text = "http://www.google.com/";
			// 
			// cacheButton
			// 
			this.cacheButton.Location = new System.Drawing.Point(296, 144);
			this.cacheButton.Name = "cacheButton";
			this.cacheButton.Size = new System.Drawing.Size(64, 24);
			this.cacheButton.TabIndex = 8;
			this.cacheButton.Text = "Cache";
			this.cacheButton.Click += new System.EventHandler(this.cacheButton_Click);
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(8, 176);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(112, 16);
			this.label3.TabIndex = 9;
			this.label3.Text = "Size of cached page:";
			this.label3.TextAlign = System.Drawing.ContentAlignment.TopRight;
			// 
			// cacheResultLabel
			// 
			this.cacheResultLabel.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.cacheResultLabel.Location = new System.Drawing.Point(120, 176);
			this.cacheResultLabel.Name = "cacheResultLabel";
			this.cacheResultLabel.Size = new System.Drawing.Size(240, 16);
			this.cacheResultLabel.TabIndex = 10;
			// 
			// spellBox
			// 
			this.spellBox.Location = new System.Drawing.Point(8, 224);
			this.spellBox.Name = "spellBox";
			this.spellBox.Size = new System.Drawing.Size(280, 20);
			this.spellBox.TabIndex = 11;
			this.spellBox.Text = "seperate pece";
			// 
			// spellButton
			// 
			this.spellButton.Location = new System.Drawing.Point(296, 224);
			this.spellButton.Name = "spellButton";
			this.spellButton.Size = new System.Drawing.Size(64, 24);
			this.spellButton.TabIndex = 12;
			this.spellButton.Text = "Spell";
			this.spellButton.Click += new System.EventHandler(this.spellButton_Click);
			// 
			// label4
			// 
			this.label4.Location = new System.Drawing.Point(8, 256);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(112, 16);
			this.label4.TabIndex = 13;
			this.label4.Text = "Spelling suggestion:";
			this.label4.TextAlign = System.Drawing.ContentAlignment.TopRight;
			// 
			// spellResultLabel
			// 
			this.spellResultLabel.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.spellResultLabel.Location = new System.Drawing.Point(120, 256);
			this.spellResultLabel.Name = "spellResultLabel";
			this.spellResultLabel.Size = new System.Drawing.Size(240, 16);
			this.spellResultLabel.TabIndex = 14;
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(376, 293);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.spellResultLabel,
																		  this.label4,
																		  this.spellButton,
																		  this.spellBox,
																		  this.cacheResultLabel,
																		  this.label3,
																		  this.cacheButton,
																		  this.cacheBox,
																		  this.searchResultLabel,
																		  this.label2,
																		  this.searchBox,
																		  this.searchButton,
																		  this.keyBox,
																		  this.label1});
			this.Name = "Form1";
			this.Text = "Google Web APIs Demo";
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// This program is a simple demo of using Google Web APIs from .NET
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new Form1());
		}

		/// <summary>
		/// Search button: do a search, display number of results 
		/// </summary>
		private void searchButton_Click(object sender, System.EventArgs e)
		{
			// Create a Google Search object
			GoogleSearchService s = new GoogleSearchService();
			// Invoke the search method
			GoogleSearchResult r = s.doGoogleSearch(keyBox.Text, searchBox.Text, 
				0, 1, false, "", false, "", "", "");
			// Extract the estimated number of results for the search and display it
			int estResults = r.estimatedTotalResultsCount;
			searchResultLabel.Text = Convert.ToString(estResults);
		}

		/// <summary>
		/// Cache button: look up a URL in the Google cache, display size of page
		/// </summary>
		private void cacheButton_Click(object sender, System.EventArgs e)
		{
			// Create a Google Search object
			GoogleSearchService s = new GoogleSearchService();
			// Invoke the doGetCachedPage method and get the cached bytes
			System.Byte[] bytes = s.doGetCachedPage(keyBox.Text, cacheBox.Text);
			// Display the length of the cached page
			cacheResultLabel.Text = Convert.ToString(bytes.Length);
		}

		/// <summary>
		/// Spell button: ask Google for a suggested alternate spelling, display it
		/// </summary>
		private void spellButton_Click(object sender, System.EventArgs e)
		{
			// Create a Google Search object
			GoogleSearchService s = new GoogleSearchService();
			// Ask for spelling suggestion
			String suggestion = s.doSpellingSuggestion(keyBox.Text, spellBox.Text);
			// Display the suggestion, if any
			if (suggestion == null)
				this.spellResultLabel.Text = "<no suggestion>";
			else
				this.spellResultLabel.Text = suggestion;
		}
	}
}
