using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

// Code scaffolded by EF Core assumes nullable reference types (NRTs) are not used or disabled.
// If you have enabled NRTs for your project, then un-comment the following line:
// #nullable disable

namespace Facecore.Models
{
    public partial class clockin
    {
        [Key]
        [Column(TypeName = "int(11)")]
        public int Id { get; set; }
        [Required]
        [StringLength(255)]
        public string stu_id { get; set; }
        [Required]
        [StringLength(50)]
        public string stu_name { get; set; }
        [Required]
        [StringLength(255)]
        public string stu_sex { get; set; }
        [StringLength(255)]
        public string come_time { get; set; }
        [StringLength(255)]
        public string leave_time { get; set; }
        [StringLength(255)]
        public string clock_time { get; set; }
        [StringLength(255)]
        public string clock_num { get; set; }
        [StringLength(255)]
        public string clock_days { get; set; }
    }
}
