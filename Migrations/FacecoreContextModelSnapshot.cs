﻿// <auto-generated />
using System;
using Facecore.Models;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Infrastructure;
using Microsoft.EntityFrameworkCore.Storage.ValueConversion;

namespace Facecore.Migrations
{
    [DbContext(typeof(FacecoreContext))]
    partial class FacecoreContextModelSnapshot : ModelSnapshot
    {
        protected override void BuildModel(ModelBuilder modelBuilder)
        {
#pragma warning disable 612, 618
            modelBuilder
                .HasAnnotation("ProductVersion", "3.1.26")
                .HasAnnotation("Relational:MaxIdentifierLength", 64);

            modelBuilder.Entity("Facecore.Models.acti", b =>
                {
                    b.Property<int>("Id")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("int(11)");

                    b.Property<string>("acti_content")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("'NULL'")
                        .HasMaxLength(255);

                    b.Property<string>("acti_head")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("'NULL'")
                        .HasMaxLength(255);

                    b.Property<string>("acti_peo")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("'NULL'")
                        .HasMaxLength(255);

                    b.Property<string>("acti_state")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("'NULL'")
                        .HasMaxLength(255);

                    b.Property<DateTime?>("acti_time")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("datetime(6)")
                        .HasDefaultValueSql("'NULL'");

                    b.HasKey("Id");

                    b.ToTable("acti");
                });

            modelBuilder.Entity("Facecore.Models.clockin", b =>
                {
                    b.Property<int>("Id")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("int(11)");

                    b.Property<string>("clock_days")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("'NULL'")
                        .HasMaxLength(255);

                    b.Property<string>("clock_num")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("'NULL'")
                        .HasMaxLength(255);

                    b.Property<string>("clock_time")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("'NULL'")
                        .HasMaxLength(255);

                    b.Property<string>("come_time")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("'NULL'")
                        .HasMaxLength(255);

                    b.Property<string>("leave_time")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("'NULL'")
                        .HasMaxLength(255);

                    b.Property<string>("stu_id")
                        .IsRequired()
                        .HasColumnType("varchar(255)")
                        .HasMaxLength(255);

                    b.Property<string>("stu_name")
                        .IsRequired()
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(50)")
                        .HasDefaultValueSql("''")
                        .HasMaxLength(50);

                    b.Property<string>("stu_sex")
                        .IsRequired()
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("''")
                        .HasMaxLength(255);

                    b.HasKey("Id");

                    b.HasIndex("stu_id")
                        .HasName("stu_id");

                    b.HasIndex("stu_name")
                        .HasName("stu_name");

                    b.HasIndex("stu_sex")
                        .HasName("stu_sex");

                    b.ToTable("clockin");
                });

            modelBuilder.Entity("Facecore.Models.grade", b =>
                {
                    b.Property<int>("Id")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("int(11)");

                    b.Property<string>("stu_errnum")
                        .IsRequired()
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("'0'")
                        .HasMaxLength(255);

                    b.Property<string>("stu_id")
                        .IsRequired()
                        .HasColumnType("varchar(255)")
                        .HasMaxLength(255);

                    b.Property<string>("stu_name")
                        .IsRequired()
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(50)")
                        .HasDefaultValueSql("''")
                        .HasMaxLength(50);

                    b.Property<string>("stu_sex")
                        .IsRequired()
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("''")
                        .HasMaxLength(255);

                    b.HasKey("Id");

                    b.HasIndex("stu_id")
                        .HasName("stu_id");

                    b.HasIndex("stu_name")
                        .HasName("stu_name");

                    b.HasIndex("stu_sex")
                        .HasName("stu_sex");

                    b.ToTable("grade");
                });

            modelBuilder.Entity("Facecore.Models.lib", b =>
                {
                    b.Property<int>("Id")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("int(11)");

                    b.Property<string>("lib_id")
                        .IsRequired()
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("''")
                        .HasMaxLength(255);

                    b.Property<string>("lib_location")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("'NULL'")
                        .HasMaxLength(255);

                    b.Property<string>("lib_name")
                        .IsRequired()
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("''")
                        .HasMaxLength(255);

                    b.Property<int>("lib_num")
                        .HasColumnType("int(11)");

                    b.HasKey("Id");

                    b.HasIndex("lib_id")
                        .HasName("lib_id");

                    b.HasIndex("lib_name")
                        .HasName("lib_name");

                    b.ToTable("lib");
                });

            modelBuilder.Entity("Facecore.Models.reserve", b =>
                {
                    b.Property<int>("Id")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("int(11)");

                    b.Property<string>("come_state")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("'NULL'")
                        .HasMaxLength(255);

                    b.Property<string>("leave_state")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("'NULL'")
                        .HasMaxLength(255);

                    b.Property<string>("lib_id")
                        .IsRequired()
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("''")
                        .HasMaxLength(255);

                    b.Property<string>("lib_name")
                        .IsRequired()
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("''")
                        .HasMaxLength(255);

                    b.Property<DateTime>("re_time")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("datetime(6)")
                        .HasDefaultValueSql("'current_timestamp(6)'");

                    b.Property<string>("stu_id")
                        .IsRequired()
                        .HasColumnType("varchar(255)")
                        .HasMaxLength(255);

                    b.Property<string>("stu_name")
                        .IsRequired()
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(50)")
                        .HasDefaultValueSql("''")
                        .HasMaxLength(50);

                    b.Property<string>("stu_sex")
                        .IsRequired()
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("''")
                        .HasMaxLength(255);

                    b.HasKey("Id");

                    b.HasIndex("lib_id")
                        .HasName("lib_id");

                    b.HasIndex("lib_name")
                        .HasName("lib_name");

                    b.HasIndex("stu_id")
                        .HasName("stu_id");

                    b.HasIndex("stu_name")
                        .HasName("stu_name");

                    b.HasIndex("stu_sex")
                        .HasName("stu_sex");

                    b.ToTable("reserve");
                });

            modelBuilder.Entity("Facecore.Models.stu", b =>
                {
                    b.Property<int>("Id")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("int(11)");

                    b.Property<string>("stu_class")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("'NULL'")
                        .HasMaxLength(255);

                    b.Property<int?>("stu_grade")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("int(11)")
                        .HasDefaultValueSql("'NULL'");

                    b.Property<string>("stu_id")
                        .IsRequired()
                        .HasColumnType("varchar(255)")
                        .HasMaxLength(255);

                    b.Property<string>("stu_major")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("'NULL'")
                        .HasMaxLength(255);

                    b.Property<string>("stu_name")
                        .IsRequired()
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(50)")
                        .HasDefaultValueSql("''")
                        .HasMaxLength(50);

                    b.Property<string>("stu_photo")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("'NULL'")
                        .HasMaxLength(255);

                    b.Property<string>("stu_sex")
                        .IsRequired()
                        .ValueGeneratedOnAdd()
                        .HasColumnType("varchar(255)")
                        .HasDefaultValueSql("''")
                        .HasMaxLength(255);

                    b.Property<string>("stu_state")
                        .HasColumnType("varchar(255)")
                        .HasMaxLength(255);

                    b.HasKey("Id");

                    b.HasIndex("stu_id")
                        .HasName("stu_id");

                    b.HasIndex("stu_name")
                        .HasName("stu_name");

                    b.HasIndex("stu_sex")
                        .HasName("stu_sex");

                    b.HasIndex("stu_name", "stu_id", "stu_sex")
                        .IsUnique()
                        .HasName("stu_pk");

                    b.ToTable("stu");
                });
#pragma warning restore 612, 618
        }
    }
}
