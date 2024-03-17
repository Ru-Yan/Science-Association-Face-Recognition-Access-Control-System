using Microsoft.EntityFrameworkCore.Migrations;

namespace Facecore.Migrations
{
    public partial class addstate : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.AddColumn<string>(
                name: "stu_state",
                table: "stu",
                maxLength: 255,
                nullable: true);
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropColumn(
                name: "stu_state",
                table: "stu");
        }
    }
}
