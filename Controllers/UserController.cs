using System;
using System.Linq;
using Facecore.Models;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;

namespace Facecore.Controllers
{
    [Route("api/[controller]/[action]")]
    [ApiController]
    public class UserController : ControllerBase
    {
        private readonly ILogger<UserController> _logger;
        private readonly FacecoreContext _context;

        public UserController(ILogger<UserController> logger, FacecoreContext context)
        {
            _logger = logger;
            _context = context;
        }

        public object getAllUser()
        {
            try
            {
                var stu = _context.stu;
                return stu;
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return StatusCode(404);
            }
            
        }

        public object getUserInfo(String uid)
        {
            try
            {
                var stu = _context.stu.First(e => e.stu_id == uid);
                return stu;
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return StatusCode(404);
            }
            
        }
    }
}
