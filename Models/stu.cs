using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

// Code scaffolded by EF Core assumes nullable reference types (NRTs) are not used or disabled.
// If you have enabled NRTs for your project, then un-comment the following line:
// #nullable disable

namespace Facecore.Models
{
    public partial class stu
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
        [Column(TypeName = "int(11)")]
        public int? stu_grade { get; set; }
        [StringLength(255)]
        public string stu_major { get; set; }
        [StringLength(255)]
        public string stu_class { get; set; }
        [StringLength(255)]
        public string stu_photo { get; set; }
        [StringLength(255)]
        public string stu_state { get; set; }
    }
}
