using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

// Code scaffolded by EF Core assumes nullable reference types (NRTs) are not used or disabled.
// If you have enabled NRTs for your project, then un-comment the following line:
// #nullable disable

namespace Facecore.Models
{
    public partial class acti
    {
        [Key]
        [Column(TypeName = "int(11)")]
        public int Id { get; set; }
        [StringLength(255)]
        public string acti_head { get; set; }
        [StringLength(255)]
        public string acti_content { get; set; }
        [StringLength(255)]
        public string acti_peo { get; set; }
        [Column(TypeName = "date")]
        public DateTime? start_time { get; set; }
        [Column(TypeName = "date")]
        public DateTime? end_time { get; set; }
        [StringLength(255)]
        public string acti_state { get; set; }
    }
}
