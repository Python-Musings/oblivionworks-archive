/* This file is part of Oblivion Mod Manager.
 * 
 * Oblivion Mod Manager is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Oblivion Mod Manager is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

using System;
using System.Drawing;
using System.Windows.Forms;

namespace OblivionModManager {
    internal partial class ImageForm : Form {
        internal ImageForm(Image i) {
            InitializeComponent();
            if(Program.IsImageAnimated(i)) {
                MessageBox.Show("Animated or multi-resolution images are not supported", "Error");
                pictureBox1.Image=null;
            } else pictureBox1.Image=i;
        }
        internal ImageForm(Image i, string text) : this(i) { Text=text; }

        private void pictureBox1_Click(object sender, EventArgs e) {
            Close();
        }
    }
}