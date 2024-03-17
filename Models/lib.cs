using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

// Code scaffolded by EF Core assumes nullable reference types (NRTs) are not used or disabled.
// If you have enabled NRTs for your project, then un-comment the following line:
// #nullable disable

namespace Facecore.Models
{
    public partial class lib
    {
        [Key]
        [Column(TypeName = "int(11)")]
        public int Id { get; set; }
        [Required]
        [StringLength(255)]
        public string lib_id { get; set; }
        [Required]
        [StringLength(255)]
        public string lib_name { get; set; }
        [StringLength(255)]
        public string lib_location { get; set; }
        [Column(TypeName = "int(11)")]
        public int lib_num { get; set; }
    }
}
