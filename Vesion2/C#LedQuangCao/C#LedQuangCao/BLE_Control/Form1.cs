using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Data.SqlClient;
namespace BLE_Control
{
    public partial class Form1 : Form
    {

        DateTime data = DateTime.Now; // get datetime
        SqlConnection connection;
        SqlCommand command;
        DataTable dataTable = new DataTable();
        SqlDataAdapter sqlDataAdapter = new SqlDataAdapter();
        string str = @"Data Source=DESKTOP-FE3R8E3\ADMIN;Initial Catalog=ledquangcao;Integrated Security=True";
        private int hours = 0;
        private int mins = 0;
        private int seconds = 0;
        public void load_data()
        {
            command = connection.CreateCommand();
            command.CommandText = "select * from led_quang_cao";
            sqlDataAdapter.SelectCommand = command;
            dataTable.Clear();
            sqlDataAdapter.Fill(dataTable);
            dataGridView1.DataSource = dataTable;
            connection.Close();


        }




        public Form1()
        {
            InitializeComponent();
            
        }

        private void button3_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            lblTimeNow.Text = DateTime.Now.ToString("HH:mm:ss");
            this.label4.Text = data.ToString();

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // TODO: This line of code loads data into the 'ledquangcaoDataSet1.led_quang_cao' table. You can move, or remove it, as needed.
            this.led_quang_caoTableAdapter.Fill(this.ledquangcaoDataSet1.led_quang_cao);
            // TODO: This line of code loads data into the 'ledquangcaoDataSet.led_quang_cao' table. You can move, or remove it, as needed.
            //this.led_quang_caoTableAdapter.Fill(this.ledquangcaoDataSet.led_quang_cao);
            timer1.Start();
            comboBox1.Text = ("COM3");
            comboBox1.DataSource = SerialPort.GetPortNames();
            //textBox1.ReadOnly = true;
            connection = new SqlConnection(str);
            connection.Open();
            load_data();

        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (!serialPort1.IsOpen)
            {

                          
             serialPort1.PortName = comboBox1.Text;
             
             serialPort1.Open();
             label3.Text = ("Connect");
             label3.ForeColor = Color.Green;
               
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            serialPort1.Close();
            label3.Text = ("Disconnect");
            label3.ForeColor = Color.Red;
        }

        private void button4_Click(object sender, EventArgs e)
        {
           

        }

        private void button6_Click(object sender, EventArgs e)
        {
           
        }

        private void button5_Click(object sender, EventArgs e)
        {
        
        }

        private void button7_Click(object sender, EventArgs e)
        {
           
        }

        private void button5_Click_1(object sender, EventArgs e)
        {
            txt_input.Text = string.Empty;
        }

        private void button4_Click_1(object sender, EventArgs e)
        {
            
            
            connection.Open();
            command = connection.CreateCommand();
            command.CommandText = "insert into led_quang_cao(info, times) values('" + txt_input.Text + "','" + lblTimeNow.Text + "')";
            command.ExecuteNonQuery();
            connection.Close();
            load_data();


        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {

        }

        private void label8_Click(object sender, EventArgs e)
        {

        }

        private void numMin_ValueChanged(object sender, EventArgs e)
        {

        }


        private void button6_Click_1(object sender, EventArgs e)
        {


            if (numMin.Value < 1 && numHour.Value <1)
            {
                MessageBox.Show("Bạn chưa nhập thời gian", "Thông báo!", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
            else
            {


                timer2.Start();
                timer1.Enabled = true;
                btnStart.Enabled = false;
                btnCancel.Enabled = true;

                
            }
            //if (serialPort1.IsOpen)
            //{
            //    serialPort1.Write(txt_input.Text);
            //}
            //else
            //{
            //    MessageBox.Show("connect com");
            //}
        
            

            






        }
    
        private void label11_Click(object sender, EventArgs e)
        {
           
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void button7_Click_1(object sender, EventArgs e)
        {
            timer2.Stop();
            btnStart.Enabled = true;
            btnCancel.Enabled = false;
            lblTimeLeft.Text = "00:00:00";
        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void btn_xoadb_Click(object sender, EventArgs e)
        {
            
            int Index = dataGridView1.CurrentCell.RowIndex;

            int id = Convert.ToInt32( dataGridView1.Rows[Index].Cells[0].FormattedValue.ToString());
            String sql = "delete from led_quang_cao where id =" +id;
            connection.Open();
            command = new SqlCommand(sql, connection);
            command.ExecuteNonQuery();
            connection.Close();
            load_data();
            
        }

        private void button3_Click_1(object sender, EventArgs e)
        {
            connection.Open();
            command = new SqlCommand("DELETE FROM led_quang_cao", connection); // xoá hết dữ liệu Table
            command.ExecuteNonQuery();
            connection.Close();
            load_data();
        }
        private string formatHour(int s)
        {
            string t = s.ToString();
            return s < 10 ? "0" + t : t;
        }
        private void timer2_Tick(object sender, EventArgs e)
        {
            String[] timetemplate = lblTimeNow.Text.Split(':');
            int hoursSys = Convert.ToInt32(timetemplate[0]);
            int minsSys = Convert.ToInt32(timetemplate[1]);
            int secSys = Convert.ToInt32(timetemplate[2]);

            int hoursN = (int)numHour.Value;
            int minsN = (int)numMin.Value;
            int secN = 60;

            hours = hoursN - hoursSys;
            mins = minsN - minsSys - 1;
            seconds = secN - secSys;
                if (hours > 0 | mins > 0 | seconds > 0)
                {
                    if (mins == 0 && hours > 0) { mins = 59; hours = hours - 1; }
                    if (seconds == 0 && mins > 0) { seconds = 60; mins = mins - 1; }
                    seconds = seconds - 1;
                }
 
           

            // Hiển thị thời gian còn lại
            lblTimeLeft.Text = string.Format("{0}:{1}:{2}", formatHour(hours), formatHour(mins), formatHour(seconds));
                
            if(lblTimeLeft.Text.Equals("00:00:00"))
            {
                

                serialPort1.Write(txt_input.Text);
                timer2.Stop();
                btnStart.Enabled = true;
                btnCancel.Enabled = false;
                lblTimeLeft.Text = "00:00:00";
            }


        }

        private void label9_Click(object sender, EventArgs e)
        {

        }

        private void lblTimeLeft_Click(object sender, EventArgs e)
        {

        }
    }
}
