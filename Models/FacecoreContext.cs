using System;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Metadata;

// Code scaffolded by EF Core assumes nullable reference types (NRTs) are not used or disabled.
// If you have enabled NRTs for your project, then un-comment the following line:
// #nullable disable

namespace Facecore.Models
{
    public partial class FacecoreContext : DbContext
    {
        public FacecoreContext()
        {
        }

        public FacecoreContext(DbContextOptions<FacecoreContext> options)
            : base(options)
        {
        }

        public virtual DbSet<acti> acti { get; set; }
        public virtual DbSet<clockin> clockin { get; set; }
        public virtual DbSet<grade> grade { get; set; }
        public virtual DbSet<lib> lib { get; set; }
        public virtual DbSet<reserve> reserve { get; set; }
        public virtual DbSet<stu> stu { get; set; }

        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            
        }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<acti>(entity =>
            {
                entity.Property(e => e.acti_content).HasDefaultValueSql("'NULL'");

                entity.Property(e => e.acti_head).HasDefaultValueSql("'NULL'");

                entity.Property(e => e.acti_peo).HasDefaultValueSql("'NULL'");

                entity.Property(e => e.acti_state).HasDefaultValueSql("'NULL'");

                entity.Property(e => e.start_time).HasDefaultValueSql("'NULL'");

                entity.Property(e => e.end_time).HasDefaultValueSql("'NULL'");
            });

            modelBuilder.Entity<clockin>(entity =>
            {
                entity.HasIndex(e => e.stu_id)
                    .HasName("stu_id");

                entity.HasIndex(e => e.stu_name)
                    .HasName("stu_name");

                entity.HasIndex(e => e.stu_sex)
                    .HasName("stu_sex");

                entity.Property(e => e.clock_days).HasDefaultValueSql("'NULL'");

                entity.Property(e => e.clock_num).HasDefaultValueSql("'NULL'");

                entity.Property(e => e.clock_time).HasDefaultValueSql("'NULL'");

                entity.Property(e => e.come_time).HasDefaultValueSql("'NULL'");

                entity.Property(e => e.leave_time).HasDefaultValueSql("'NULL'");

                entity.Property(e => e.stu_name).HasDefaultValueSql("''");

                entity.Property(e => e.stu_sex).HasDefaultValueSql("''");
            });

            modelBuilder.Entity<grade>(entity =>
            {
                entity.HasIndex(e => e.stu_id)
                    .HasName("stu_id");

                entity.HasIndex(e => e.stu_name)
                    .HasName("stu_name");

                entity.HasIndex(e => e.stu_sex)
                    .HasName("stu_sex");

                entity.Property(e => e.stu_errnum).HasDefaultValueSql("'0'");

                entity.Property(e => e.stu_name).HasDefaultValueSql("''");

                entity.Property(e => e.stu_sex).HasDefaultValueSql("''");
            });

            modelBuilder.Entity<lib>(entity =>
            {
                entity.HasIndex(e => e.lib_id)
                    .HasName("lib_id");

                entity.HasIndex(e => e.lib_name)
                    .HasName("lib_name");

                entity.Property(e => e.lib_id).HasDefaultValueSql("''");

                entity.Property(e => e.lib_location).HasDefaultValueSql("'NULL'");

                entity.Property(e => e.lib_name).HasDefaultValueSql("''");
            });

            modelBuilder.Entity<reserve>(entity =>
            {
                entity.HasIndex(e => e.lib_id)
                    .HasName("lib_id");

                entity.HasIndex(e => e.lib_name)
                    .HasName("lib_name");

                entity.HasIndex(e => e.stu_id)
                    .HasName("stu_id");

                entity.HasIndex(e => e.stu_name)
                    .HasName("stu_name");

                entity.HasIndex(e => e.stu_sex)
                    .HasName("stu_sex");

                entity.Property(e => e.come_state).HasDefaultValueSql("'NULL'");

                entity.Property(e => e.leave_state).HasDefaultValueSql("'NULL'");

                entity.Property(e => e.lib_id).HasDefaultValueSql("''");

                entity.Property(e => e.lib_name).HasDefaultValueSql("''");

                entity.Property(e => e.re_time).HasDefaultValueSql("'current_timestamp(6)'");

                entity.Property(e => e.stu_name).HasDefaultValueSql("''");

                entity.Property(e => e.stu_sex).HasDefaultValueSql("''");
            });

            modelBuilder.Entity<stu>(entity =>
            {
                entity.HasIndex(e => e.stu_id)
                    .HasName("stu_id");

                entity.HasIndex(e => e.stu_name)
                    .HasName("stu_name");

                entity.HasIndex(e => e.stu_sex)
                    .HasName("stu_sex");

                entity.HasIndex(e => new { e.stu_name, e.stu_id, e.stu_sex })
                    .HasName("stu_pk")
                    .IsUnique();

                entity.Property(e => e.stu_class).HasDefaultValueSql("'NULL'");

                entity.Property(e => e.stu_grade).HasDefaultValueSql("'NULL'");

                entity.Property(e => e.stu_major).HasDefaultValueSql("'NULL'");

                entity.Property(e => e.stu_name).HasDefaultValueSql("''");

                entity.Property(e => e.stu_photo).HasDefaultValueSql("'NULL'");

                entity.Property(e => e.stu_sex).HasDefaultValueSql("''");
            });

            OnModelCreatingPartial(modelBuilder);
        }

        partial void OnModelCreatingPartial(ModelBuilder modelBuilder);
    }
}
