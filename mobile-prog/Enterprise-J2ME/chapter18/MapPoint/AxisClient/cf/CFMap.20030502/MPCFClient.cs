using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.Data;
using CFMap.MPService;
using System.IO;

namespace CFMap
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class CFMapForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.TabPage directionsPage;
		private System.Windows.Forms.TabPage mapPage;
		private System.Windows.Forms.ListBox directionsList;
		private System.Windows.Forms.PictureBox mapBox;
		private System.Windows.Forms.TextBox fromZip;
		private System.Windows.Forms.TextBox fromState;
		private System.Windows.Forms.TextBox fromCity;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TextBox fromStreet;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TabPage fromPage;
		private System.Windows.Forms.TabPage toPage;
		private System.Windows.Forms.TextBox toZip;
		private System.Windows.Forms.TextBox toState;
		private System.Windows.Forms.TextBox toCity;
		private System.Windows.Forms.TextBox toStreet;
		private System.Windows.Forms.Label label9;
		private System.Windows.Forms.Label State;
		private System.Windows.Forms.Label label8;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.Label label10;
		private System.Windows.Forms.Label label11;
		private System.Windows.Forms.MainMenu mainMenu1;
		private System.Windows.Forms.Button btnGetDirections;
		private System.Windows.Forms.Button btnGetMap;

		private MPClientService service;

		public CFMapForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
			service = new MPClientService ();
		}
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			base.Dispose( disposing );
		}
		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.mainMenu1 = new System.Windows.Forms.MainMenu();
			this.tabControl1 = new System.Windows.Forms.TabControl();
			this.fromPage = new System.Windows.Forms.TabPage();
			this.label10 = new System.Windows.Forms.Label();
			this.fromZip = new System.Windows.Forms.TextBox();
			this.fromState = new System.Windows.Forms.TextBox();
			this.fromCity = new System.Windows.Forms.TextBox();
			this.label5 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.fromStreet = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.toPage = new System.Windows.Forms.TabPage();
			this.label11 = new System.Windows.Forms.Label();
			this.toZip = new System.Windows.Forms.TextBox();
			this.toState = new System.Windows.Forms.TextBox();
			this.toCity = new System.Windows.Forms.TextBox();
			this.toStreet = new System.Windows.Forms.TextBox();
			this.label9 = new System.Windows.Forms.Label();
			this.State = new System.Windows.Forms.Label();
			this.label8 = new System.Windows.Forms.Label();
			this.label7 = new System.Windows.Forms.Label();
			this.label6 = new System.Windows.Forms.Label();
			this.directionsPage = new System.Windows.Forms.TabPage();
			this.btnGetDirections = new System.Windows.Forms.Button();
			this.btnGetMap = new System.Windows.Forms.Button();
			this.directionsList = new System.Windows.Forms.ListBox();
			this.mapPage = new System.Windows.Forms.TabPage();
			this.mapBox = new System.Windows.Forms.PictureBox();
			// 
			// tabControl1
			// 
			this.tabControl1.Controls.Add(this.fromPage);
			this.tabControl1.Controls.Add(this.toPage);
			this.tabControl1.Controls.Add(this.directionsPage);
			this.tabControl1.Controls.Add(this.mapPage);
			this.tabControl1.Font = new System.Drawing.Font("Tahoma", 10F, System.Drawing.FontStyle.Regular);
			this.tabControl1.SelectedIndex = 0;
			this.tabControl1.Size = new System.Drawing.Size(240, 272);
			// 
			// fromPage
			// 
			this.fromPage.Controls.Add(this.label10);
			this.fromPage.Controls.Add(this.fromZip);
			this.fromPage.Controls.Add(this.fromState);
			this.fromPage.Controls.Add(this.fromCity);
			this.fromPage.Controls.Add(this.label5);
			this.fromPage.Controls.Add(this.label4);
			this.fromPage.Controls.Add(this.label3);
			this.fromPage.Controls.Add(this.label2);
			this.fromPage.Controls.Add(this.fromStreet);
			this.fromPage.Controls.Add(this.label1);
			this.fromPage.Location = new System.Drawing.Point(4, 4);
			this.fromPage.Size = new System.Drawing.Size(232, 243);
			this.fromPage.Text = "From";
			// 
			// label10
			// 
			this.label10.Location = new System.Drawing.Point(16, 152);
			this.label10.Size = new System.Drawing.Size(176, 40);
			this.label10.Text = "Please fill out the To address and go to Directions tab";
			// 
			// fromZip
			// 
			this.fromZip.Location = new System.Drawing.Point(152, 96);
			this.fromZip.Size = new System.Drawing.Size(56, 22);
			this.fromZip.Text = "";
			// 
			// fromState
			// 
			this.fromState.Location = new System.Drawing.Point(64, 96);
			this.fromState.Size = new System.Drawing.Size(48, 22);
			this.fromState.Text = "";
			// 
			// fromCity
			// 
			this.fromCity.Location = new System.Drawing.Point(40, 64);
			this.fromCity.Size = new System.Drawing.Size(168, 22);
			this.fromCity.Text = "";
			// 
			// label5
			// 
			this.label5.Location = new System.Drawing.Point(120, 104);
			this.label5.Size = new System.Drawing.Size(24, 20);
			this.label5.Text = "Zip";
			// 
			// label4
			// 
			this.label4.Location = new System.Drawing.Point(8, 104);
			this.label4.Size = new System.Drawing.Size(40, 20);
			this.label4.Text = "State";
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(8, 72);
			this.label3.Text = "City";
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(8, 40);
			this.label2.Size = new System.Drawing.Size(48, 20);
			this.label2.Text = "Street";
			// 
			// fromStreet
			// 
			this.fromStreet.Location = new System.Drawing.Point(56, 32);
			this.fromStreet.Size = new System.Drawing.Size(152, 22);
			this.fromStreet.Text = "";
			// 
			// label1
			// 
			this.label1.Font = new System.Drawing.Font("Tahoma", 12F, System.Drawing.FontStyle.Bold);
			this.label1.Location = new System.Drawing.Point(16, 8);
			this.label1.Size = new System.Drawing.Size(136, 20);
			this.label1.Text = "From Address";
			// 
			// toPage
			// 
			this.toPage.Controls.Add(this.label11);
			this.toPage.Controls.Add(this.toZip);
			this.toPage.Controls.Add(this.toState);
			this.toPage.Controls.Add(this.toCity);
			this.toPage.Controls.Add(this.toStreet);
			this.toPage.Controls.Add(this.label9);
			this.toPage.Controls.Add(this.State);
			this.toPage.Controls.Add(this.label8);
			this.toPage.Controls.Add(this.label7);
			this.toPage.Controls.Add(this.label6);
			this.toPage.Location = new System.Drawing.Point(4, 4);
			this.toPage.Size = new System.Drawing.Size(232, 243);
			this.toPage.Text = "To";
			// 
			// label11
			// 
			this.label11.Location = new System.Drawing.Point(16, 144);
			this.label11.Size = new System.Drawing.Size(176, 40);
			this.label11.Text = "Please fill out the From address and go to Directions tab";
			// 
			// toZip
			// 
			this.toZip.Location = new System.Drawing.Point(152, 96);
			this.toZip.Size = new System.Drawing.Size(56, 22);
			this.toZip.Text = "";
			// 
			// toState
			// 
			this.toState.Location = new System.Drawing.Point(64, 96);
			this.toState.Size = new System.Drawing.Size(48, 22);
			this.toState.Text = "";
			// 
			// toCity
			// 
			this.toCity.Location = new System.Drawing.Point(40, 64);
			this.toCity.Size = new System.Drawing.Size(168, 22);
			this.toCity.Text = "";
			// 
			// toStreet
			// 
			this.toStreet.Location = new System.Drawing.Point(56, 32);
			this.toStreet.Size = new System.Drawing.Size(152, 22);
			this.toStreet.Text = "";
			// 
			// label9
			// 
			this.label9.Location = new System.Drawing.Point(120, 104);
			this.label9.Size = new System.Drawing.Size(24, 20);
			this.label9.Text = "Zip";
			// 
			// State
			// 
			this.State.Location = new System.Drawing.Point(8, 104);
			this.State.Size = new System.Drawing.Size(40, 20);
			this.State.Text = "State";
			// 
			// label8
			// 
			this.label8.Location = new System.Drawing.Point(8, 72);
			this.label8.Size = new System.Drawing.Size(32, 20);
			this.label8.Text = "City";
			// 
			// label7
			// 
			this.label7.Location = new System.Drawing.Point(8, 40);
			this.label7.Size = new System.Drawing.Size(40, 20);
			this.label7.Text = "Street";
			// 
			// label6
			// 
			this.label6.Font = new System.Drawing.Font("Tahoma", 12F, System.Drawing.FontStyle.Bold);
			this.label6.Location = new System.Drawing.Point(8, 8);
			this.label6.Text = "To Address";
			// 
			// directionsPage
			// 
			this.directionsPage.Controls.Add(this.btnGetDirections);
			this.directionsPage.Controls.Add(this.btnGetMap);
			this.directionsPage.Controls.Add(this.directionsList);
			this.directionsPage.Location = new System.Drawing.Point(4, 4);
			this.directionsPage.Size = new System.Drawing.Size(232, 243);
			this.directionsPage.Text = "Directions";
			// 
			// btnGetDirections
			// 
			this.btnGetDirections.Location = new System.Drawing.Point(8, 216);
			this.btnGetDirections.Size = new System.Drawing.Size(104, 20);
			this.btnGetDirections.Text = "Get Directions";
			this.btnGetDirections.Click += new System.EventHandler(this.btnGetDirections_Click);
			// 
			// btnGetMap
			// 
			this.btnGetMap.Location = new System.Drawing.Point(152, 216);
			this.btnGetMap.Text = "Get Map";
			this.btnGetMap.Click += new System.EventHandler(this.btnGetMap_Click);
			// 
			// directionsList
			// 
			this.directionsList.Location = new System.Drawing.Point(8, 8);
			this.directionsList.Size = new System.Drawing.Size(216, 198);
			// 
			// mapPage
			// 
			this.mapPage.Controls.Add(this.mapBox);
			this.mapPage.Location = new System.Drawing.Point(4, 4);
			this.mapPage.Size = new System.Drawing.Size(232, 243);
			this.mapPage.Text = "Map";
			// 
			// mapBox
			// 
			this.mapBox.Location = new System.Drawing.Point(8, 8);
			this.mapBox.Size = new System.Drawing.Size(216, 216);
			// 
			// CFMapForm
			// 
			this.Controls.Add(this.tabControl1);
			this.Menu = this.mainMenu1;
			this.Text = "MapPoint.NET Client";

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>

		static void Main() 
		{
			Application.Run(new CFMapForm());
		}

		private void btnGetMap_Click(object sender, System.EventArgs e)
		{
			byte [] imgarray = service.getMap(directionsList.SelectedIndex, 200, 200);
			mapBox.Image = new Bitmap (new MemoryStream(imgarray));
			mapPage.BringToFront();
		}

		private void btnGetDirections_Click(object sender, System.EventArgs e)
		{
			String [] directions = service.getDirections(
				fromStreet.Text, fromCity.Text, fromState.Text, fromZip.Text, 
                toStreet.Text, toCity.Text, toState.Text, toZip.Text);
			ArrayList al = new ArrayList ();
			al.Add("Overview Map");
			for (int i = 0; i < directions.Length; i++ ) 
			{
				al.Add( directions[i] );
			}
			directionsList.DataSource = al;
		}
	}
}
